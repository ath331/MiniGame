#pragma once
#include <WinSock2.h>

enum class PacketIndex : int
{
	// for tesing code
	ECHO,
	CS_EnterRoom,	//�뿡 ������ �� ������ ��Ŷ
	SC_EnterRoom,	//���� �뿡 �ٸ� �÷��̾ �������� �� �ش� �濡 �ִ� �������� ������ ��Ŷ
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