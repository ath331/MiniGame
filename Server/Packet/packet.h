#pragma once
#include <WinSock2.h>

enum class PacketIndex : int
{
	// for tesing code
	ECHO,
	CS_EnterRoom,	//�뿡 ������ �� ������ ��Ŷ �ش� ��Ŷ�� ����ü�� ���� ������ �ʰ� SC_EnterRoom �۽Ÿ� ���ش�.
	SC_EnterRoom,	//CS_EnterRoom�� ���� Ŭ�� �����Ͽ� �濡 ��� Ŭ�󿡰� �����ϴ� ��Ŷ. ���� �ο��� ����̰� �ش� �ο��鿡 ���� ���������� ������.
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

const static int maxClientCount = 3; //RoomManager.h ��const static int maxClientCountInRoom�� ���� ���ε� ��� ������ �ؾ� ���� ������ ���� �ϴ� �ΰ��� ������
struct SC_PacketEnterRoom : public PacketHeader
{
	int maxUserinRoom = 0; //������ ���� ���� ��
	int sock[maxClientCount] {0,}; //������ SOCKET ��ȣ�� ���� �迭

	SC_PacketEnterRoom()
	{
		headerSize = sizeof(SC_PacketEnterRoom);
	}
};