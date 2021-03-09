#include "ThreadManager.h"
#include "ThreadManagerCppHeader.h"
#include "OverlappedCustom.h"
#include "Basic.h"
#include "../ClientManager.h"

#include <process.h>
#include <algorithm>

void ThreadManager::InitThreadManager(int maxThreadNum, HANDLE comPort, Acceptor* acceptor, ClientManager* clientManager)
{
	_maxThreadNum = maxThreadNum;
	_comPort = comPort;

	if (!IsNullPtr(acceptor))
		_acceptor = acceptor;
	if (!IsNullPtr(clientManager))
		_clientManager = clientManager;
}

void ThreadManager::MakeThread()
{
	_MakeIOThreads();
	_MakeLogicThread();
}

void ThreadManager::_MakeIOThreads()
{
	for (int i = 0; i < _maxThreadNum - 2; i++)
	{
		HANDLE threadHandle = (HANDLE)_beginthreadex(NULL, 0, _RunIOThreadMain, this, 0, NULL);
		_threadHandleVec.push_back(threadHandle);
	}
}

void ThreadManager::_MakeLogicThread()
{
	HANDLE threadHandle = (HANDLE)_beginthreadex(NULL, 0, _RunLogicThreadMain, this, 0, NULL);
	_threadHandleVec.push_back(threadHandle);
}

void ThreadManager::WaitThread()
{
	if (_threadHandleVec.empty() == false)
		WaitForMultipleObjects(static_cast<DWORD>(_threadHandleVec.size()), &_threadHandleVec[0], true, INFINITE);
}

unsigned int WINAPI ThreadManager::_RunIOThreadMain(void* _thisObject)
{
	if (IsNullPtr(_thisObject))
		return -1;

	ThreadManager* thisObject = (ThreadManager*)_thisObject;
	DWORD bytesTrans = 0;
	Overlapped* ioInfo = nullptr;
	SOCKET sock = 0; //CompletionKey

	while (1)
	{
		GetQueuedCompletionStatus(thisObject->_comPort, &bytesTrans, (PULONG_PTR)&sock, (LPOVERLAPPED*)&ioInfo, INFINITE);

		if (ioInfo->ioType == Overlapped::IO_TYPE::ACCEPT) //Client 접속
		{
			SOCKET sock = ioInfo->sock; //접속한 clientSock
			TcpSession* session = new TcpSession(thisObject->_comPort, sock, &thisObject->_packetQueue);
			CLIENT_MANAGER->PushClient(sock, session);
			session->PostRecv();

			thisObject->_acceptor->AcceptClient();

			Log log(LogIndex::LOG, "Client Accept");
		}

		else if (ioInfo->ioType == Overlapped::IO_TYPE::RECV) //Client의 데이터 Recv
		{
			if (bytesTrans == 0)
			{
				CLIENT_MANAGER->PopClient(sock);
				continue;
			}
			CLIENT_MANAGER->GetSession(sock)->OnRecvForIocp(bytesTrans);
		}

		else if (ioInfo->ioType == Overlapped::IO_TYPE::SEND) //Client에게 데이터 Send
		{
			CLIENT_MANAGER->GetSession(sock)->OnSendForIocp();
		}

	}
	return 0;
}

unsigned int WINAPI ThreadManager::_RunLogicThreadMain(void* _thisObject)
{
	if (IsNullPtr(_thisObject))
		return -1;

	ThreadManager* thisObject = static_cast<ThreadManager*>(_thisObject);
	PacketInfo packetInfo;
	TcpSession* clientSession = nullptr;
	while (true)
	{
		Sleep(1);
		if (thisObject->_packetQueue.try_pop(packetInfo))
		{
			clientSession = CLIENT_MANAGER->GetSession(packetInfo.sock);
			switch (packetInfo.packetIndex)
			{
			case PacketIndex::ECHO:
			{
				PacketEcho packetEcho;
				memcpy(&packetEcho, packetInfo.packetBuffer, sizeof(PacketEcho));
				clientSession->PushSendVec(packetInfo, sizeof(PacketEcho));
			}
			break;

			default:
				break;
			}
		}
	}
	return 0;
}