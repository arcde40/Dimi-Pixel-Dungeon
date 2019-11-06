#pragma once
#include "GlobalVariable.h"
#include "pointarraylist.h"
#include "struct.h"
#include "map.h"
#include "render.h"
#include "MobList.h"
#include <stdio.h>
#include <math.h>


PointArrayList* findPath(int map[][MIN_Y+MAX_Y], int startPosX, int startPosY, int endPosX, int endPosY, MobList* list, int step);
int getH(int x1, int y1, int x2, int y2);