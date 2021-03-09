#include "ClientManager.h"
#include "Basic.h"

void ClientManager::PushClient(SOCKET sock, TcpSession* session)
{
	if (IsNullPtr(session))
		return;

	ClientState cs;
	cs.session = session;
	_clientMap.insert(make_pair(sock, cs));
}

void ClientManager::PopClient(SOCKET sock)
{
	SafeDeletePtr(_clientMap[sock].session);
	_clientMap.erase(sock);

	Log log(LogIndex::LOG, "Client Close");
}

TcpSession* ClientManager::GetSessionInClientMap(SOCKET sock)
{
	auto pair = _clientMap.find(sock);
	if (pair == _clientMap.end())
		return nullptr;

	return _clientMap[sock].session;
}

ClientState& ClientManager::GetClientStateInClientMap(SOCKET sock)
{
	/*auto pair = _clientMap.find(sock);
	if (pair == _clientMap.end())
		return nullptr;*/

	return _clientMap[sock];
}

