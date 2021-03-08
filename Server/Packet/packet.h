#pragma once
//server <-> client 통신관련 packet 저장
//RES_ is Server Packet
#include <WinSock2.h>

const static int MAX_NAME_LENGTH = 10;
const static int MAX_ROOM_NAME_LENGTH = 20;
const static int MAX_ROOM_COUNT = 20;


enum class PacketIndex : int
{
	// for tesing code
	ECHO,
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