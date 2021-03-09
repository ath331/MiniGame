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
		bool isEnterable = true; //방이 현재 입장가능 상태인지 나타내는 변수
	};

public:
	bool EnterRoom(ClientState& clientState);

private:
	Room _MakeRoom();
	int _AccessibleRoomNum(); //입장가능한 방의 번호를 반환한다. (최대 방의 갯수만큼 방이 생성되어 있고 모든 방에 최대 유저가 들어가있다면 = 입장가능한 방이 없다면-1 리턴 )

	unordered_map<int, Room> _roomMap;
};

