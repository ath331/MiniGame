#include "ClientManager.h"
#include "Basic.h"

void ClientManager::PushClient(SOCKET sock, TcpSession* session)
{
	if (!IsNullPtr(session))
		_clientMap[sock] = session;
}

void ClientManager::PopClient(SOCKET sock)
{
	DeleteSafePtr(_clientMap[sock]);
	_clientMap.erase(sock);

	Log log(LogIndex::LOG, "Client Close");
}

TcpSession* ClientManager::GetSession(SOCKET sock)
{
	return _clientMap[sock];
}
