#pragma once
#include "room.h"
#include "arraylist.h"
#include "roomarraylist.h"
#include <time.h>
#include <math.h>
#include <Windows.h>


#include "GlobalVariable.h"

void draw(int map[][MIN_Y+MAX_Y], RoomArrayList* arr);
bool isPassable(int i);
COORD getStartPos();
COORD getEndPos();