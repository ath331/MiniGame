#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <concurrent_queue.h>

#include "packet.h"

using namespace std;

class ClientManager;
class Acceptor;
class RoomManager;
class ThreadManager
{
public:
	void InitThreadManager(int maxThreadNum, HANDLE comPort, ClientManager* clientManager, Acceptor* accept, RoomManager* roomManager);
	void MakeThread();
	void WaitThread();
private:

	HANDLE _comPort;
	vector<HANDLE> _threadHandleVec;
	int _maxThreadNum = 0;

	void _MakeIOThreads();
	void _MakeLogicThread();
	void _MakeDBThread();

	static unsigned int WINAPI _RunIOThreadMain(void* thisObject); //IOCPó�� ��Ƽ������
	static unsigned int WINAPI _RunLogicThreadMain(void* thisObject); //Logicó�� �̱۽�����
	static unsigned int WINAPI _RunDBThreadMain(void* thisObject); //DBó�� �̱۽�����

	concurrency::concurrent_queue<PacketInfo> _packetDBQueue;
	concurrency::concurrent_queue<PacketInfo> _packetQueue;

	void _SendMessageToClient(SOCKET sock, const char* pckBuf);

	ClientManager* _clientManager;
	Acceptor* _acceptor;
	RoomManager* _roomManager;
};
