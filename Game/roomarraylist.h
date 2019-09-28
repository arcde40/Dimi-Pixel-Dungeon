#pragma once
#include "room.h"

typedef struct RoomArrayList {
	int size;
	Room* data[100];
}RoomArrayList;

RoomArrayList* initRoomArray();
void putRoom(RoomArrayList* arr, Room* value);
void removeRoom(RoomArrayList* arr, Room* index);
Room* getRoom(RoomArrayList* arr, int idx);
bool isRoomArrayEmpty(RoomArrayList* arr);