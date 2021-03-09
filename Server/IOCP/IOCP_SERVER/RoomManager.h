#pragma once
#include <unordered_map>
#include <vector>
#include "ClientState.h"

const static int maxClientCountInRoom = 3;
const static int maxRoomCount = 100;

#define ROOM_MANAGER thisObject->_roomManager

class RoomManager
{
private:
	struct Room
	{
		unsigned int roomNum = 0;
		vector<ClientState> clientStateVec;
		bool isEnterable = true; //���� ���� ���尡�� �������� ��Ÿ���� ����
	};

public:
	bool EnterRoom(ClientState& clientState);

private:
	Room _MakeRoom();
	int _AccessibleRoomNum(); //���尡���� ���� ��ȣ�� ��ȯ�Ѵ�. (�ִ� ���� ������ŭ ���� �����Ǿ� �ְ� ��� �濡 �ִ� ������ ���ִٸ� = ���尡���� ���� ���ٸ�-1 ���� )

	unordered_map<int, Room> _roomMap;
};

