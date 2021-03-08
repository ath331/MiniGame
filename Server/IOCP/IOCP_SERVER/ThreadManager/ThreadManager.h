#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <concurrent_queue.h>

#include "packet.h"

using namespace std;

class Acceptor;
class RoomManager;
class ThreadManager
{
public:
	void InitThreadManager(int maxThreadNum, HANDLE comPort, Acceptor* accept, RoomManager* roomManager);
	void MakeThread();
	void WaitThread();
private:

	HANDLE _comPort;
	vector<HANDLE> _threadHandleVec;
	int _maxThreadNum = 0;

	void _MakeIOThreads();
	void _MakeLogicThread();

	static unsigned int WINAPI _RunIOThreadMain(void* thisObject); //IO처리 멀티스레드
	static unsigned int WINAPI _RunLogicThreadMain(void* thisObject); //Logic처리 싱글스레드

	concurrency::concurrent_queue<PacketInfo> _packetQueue;

	Acceptor* _acceptor;
	RoomManager* _roomManager;
};

