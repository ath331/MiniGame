#pragma once
#include <WinSock2.h>

enum class PacketIndex : int
{
	// for tesing code
	ECHO,
	CS_EnterRoom,	//룸에 입장할 때 보내는 패킷 해당 패킷은 구조체로 따로 만들지 않고 SC_EnterRoom 송신만 해준다.
	SC_EnterRoom,	//CS_EnterRoom를 보낸 클라를 포함하여 방에 모든 클라에게 전송하는 패킷. 현재 인원이 몇명이고 해당 인원들에 대한 소켓정보를 보낸다.
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

const static int maxClientCount = 3; //RoomManager.h 에const static int maxClientCountInRoom와 같은 값인데 어디에 선언을 해야 같이 쓰일지 몰라서 일단 두개로 선언함
struct SC_PacketEnterRoom : public PacketHeader
{
	int maxUserinRoom = 0; //접속한 방의 유저 수
	int sock[maxClientCount] {0,}; //유저의 SOCKET 번호를 담은 배열

	SC_PacketEnterRoom()
	{
		headerSize = sizeof(SC_PacketEnterRoom);
	}
};