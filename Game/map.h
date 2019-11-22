#pragma once
#include "room.h"
#include "arraylist.h"
#include "roomarraylist.h"
#include "MobList.h"
#include <time.h>
#include <math.h>
#include <Windows.h>


#include "GlobalVariable.h"

void draw(int map[][MIN_Y+MAX_Y], RoomArrayList* arr);
bool isPassable(int i);
bool isTransparent(int tile);
bool isOverlapping(int x, int y, MobList* mobList);
COORD getStartPos();
COORD getEndPos();
void BitMapMapper(int map[][MIN_Y + MAX_Y], int BitMapMapping[][MIN_Y + MAX_Y]);