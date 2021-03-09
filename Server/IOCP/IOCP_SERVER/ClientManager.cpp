#include "ClientManager.h"
#include "Basic.h"

void ClientManager::PushClient(SOCKET sock, TcpSession* session)
{
	if (IsNullPtr(session))
		return;

	ClientState cs;
	cs.session = session;
	_clientMap.insert(make_pair(sock,cs));
}

void ClientManager::PopClient(SOCKET sock)
{
	SafeDeletePtr(_clientMap[sock].session);
	_clientMap.erase(sock);

	Log log(LogIndex::LOG, "Client Close");
}

TcpSession* ClientManager::GetSessionInClientMap(SOCKET sock)
{
	return _clientMap[sock].session;
}
