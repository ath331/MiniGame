#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <concurrent_queue.h>

#include "packet.h"

using namespace std;

class Acceptor;
class ClientManager;
class ThreadManager
{
public:
	void InitThreadManager(int maxThreadNum, HANDLE comPort, Acceptor* accept, ClientManager* clientManager);
	void MakeThread();
	void WaitThread();
private:

	HANDLE _comPort;
	vector<HANDLE> _threadHandleVec;
	int _maxThreadNum = 0;

	void _MakeIOThreads();
	void _MakeLogicThread();

	static unsigned int WINAPI _RunIOThreadMain(void* thisObject); //IOó�� ��Ƽ������
	static unsigned int WINAPI _RunLogicThreadMain(void* thisObject); //Logicó�� �̱۽�����

	concurrency::concurrent_queue<PacketInfo> _packetQueue;

	Acceptor* _acceptor;
	ClientManager* _clientManager;
};

