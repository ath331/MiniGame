#pragma once
#include <WinSock2.h>

enum class PacketIndex : int
{
	// for tesing code
	ECHO,
	CS_EnterRoom,	//룸에 입장할 때 보내는 패킷
	SC_EnterRoom,	//같은 룸에 다른 플레이어가 접속했을 때 해당 방에 있는 유저에게 보내는 패킷
};

struct PacketInfo
{
	SOCKET sock;
	PacketIndex packetIndex;
	const char* packetBuffer;
};

struct PacketHeader
{
	PacketIndex index = (PacketIndex)-1;
	int headerSize = sizeof(PacketHeader);

	virtual ~PacketHeader() {};
};

struct PacketEcho : public PacketHeader
{
	const char msg[20] = "";
	int uniqueNum = 0;

	PacketEcho()
	{
		index = PacketIndex::ECHO;
		uniqueNum = 0;
		headerSize = sizeof(PacketEcho);
	}
};

struct SC_PacketEnterRoom : public PacketHeader
{


	SC_PacketEnterRoom()
	{
		headerSize = sizeof(SC_PacketEnterRoom);
	}
};