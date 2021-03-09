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
		return 0;

	ThreadManager* thisObject = (ThreadManager*)_thisObject;
	DWORD bytesTrans = 0;
	Overlapped* ioInfo = nullptr;
	SOCKET sock = 0; //CompletionKey

	while (1)
	{
		GetQueuedCompletionStatus(thisObject->_comPort, &bytesTrans, (PULONG_PTR)&sock, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock = ioInfo->sock; //접속한 clientSock

		if (IsNullPtr(CLIENT_MANAGER))
			CLIENT_MANAGER = new ClientManager;

		TcpSession* session = nullptr;
		session = CLIENT_MANAGER->GetSessionInClientMap(sock);
		if (IsNullPtr(session))
			return 0;

		if (ioInfo->ioType == Overlapped::IO_TYPE::ACCEPT) //Client 접속
		{
			session = new TcpSession(thisObject->_comPort, sock, &thisObject->_packetQueue);
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

			session->OnRecvForIocp(bytesTrans);
		}

		else if (ioInfo->ioType == Overlapped::IO_TYPE::SEND) //Client에게 데이터 Send
		{
			session->OnSendForIocp();
		}

	}
	return 0;
}

unsigned int WINAPI ThreadManager::_RunLogicThreadMain(void* _thisObject)
{
	if (IsNullPtr(_thisObject))
		return 0;

	ThreadManager* thisObject = static_cast<ThreadManager*>(_thisObject);
	PacketInfo packetInfo;
	TcpSession* session = nullptr;

	while (true)
	{
		Sleep(1);
		if (thisObject->_packetQueue.try_pop(packetInfo))
		{
			session = CLIENT_MANAGER->GetSessionInClientMap(packetInfo.sock);
			if (IsNullPtr(session))
				return 0;

			switch (packetInfo.packetIndex)
			{
			case PacketIndex::ECHO:
			{
				PacketEcho packetEcho;
				memcpy(&packetEcho, packetInfo.packetBuffer, sizeof(PacketEcho));
				session->PushSendVec(packetInfo, sizeof(PacketEcho));
			}
			break;

			default:
				break;
			}
		}
	}
	return 0;
}