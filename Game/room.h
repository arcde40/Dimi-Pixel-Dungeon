#ifndef ROOM_HEADER
#define ROOM_HEADER
#include "arraylist.h"

typedef struct Room {
	int idx;
	int weight, height, centerX, centerY;
	int corridorCount;
	ArrayList* connected;
} Room;

Room* init(int idx, int weight, int height, int centerX, int centerY, int corridorCount);
bool intersect(Room* x, Room* y);
#endif