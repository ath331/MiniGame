#pragma once
#include <WinSock2.h>
#include <unordered_map>

#include "Server/Session/TcpSession.h"

#define CLIENT_MANAGER thisObject->_clientManager  //ThreadManager에서 사용하는 매크로

struct ClientState
{
	TcpSession* session;
};

class ClientManager
{
public:
	void PushClient(SOCKET sock, TcpSession* session);
	void PopClient(SOCKET sock);
	TcpSession* GetSession(SOCKET sock);

private:
	unordered_map<SOCKET, ClientState> _clientMap;
};

