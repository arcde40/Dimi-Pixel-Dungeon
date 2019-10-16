#pragma once
#include "pointarraylist.h"
#include "GlobalVariable.h"

PointArrayList* findPath(int map[][MIN_Y+MAX_Y], int startPosX, int startPosY, int endPosX, int endPosY);
int getH(int x1, int y1, int x2, int y2);