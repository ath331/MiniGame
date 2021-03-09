#include "RoomManager.h"

RoomManager::Room RoomManager::_MakeRoom()
{
	int roomNum = _AccessibleRoomNum();
	Room room;
	room.roomNum = roomNum;

	return room;
}

bool RoomManager::EnterRoom(ClientState& clientState)
{
	for (auto iter = _roomMap.begin(); iter != _roomMap.end(); iter++) //생성되어 있는 방 중에서 입장
	{
		if (iter->second.clientStateVec.size() < maxClientCountInRoom)
		{
			iter->second.clientStateVec.push_back(clientState);
			if (iter->second.clientStateVec.size() >= maxClientCountInRoom)
				iter->second.isEnterable = false;

			return true;
		}
	}

	if (_roomMap.size() > maxRoomCount)
		return false;

	Room room = _MakeRoom();
	room.clientStateVec.push_back(clientState);

	_roomMap.insert(make_pair(room.roomNum, room));

	return true;
}

int RoomManager::_AccessibleRoomNum()
{
	if (_roomMap.size() > maxRoomCount)
		return -1;

	for (auto iter = _roomMap.begin(); iter != _roomMap.end(); iter++)
	{
		if (iter->second.isEnterable)
		{
			return iter->second.roomNum;
		}
	}

	return -1;
}