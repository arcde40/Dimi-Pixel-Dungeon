#pragma once
#include "pathfining.h"
#include "lighting.h"
#include "MobList.h"
#include "Player.h"

PointArrayList* enemyBehave(MobInfo* mobInfo, int playerX, int playerY, int map[][MAX_Y + MIN_Y], Player* p);
void moveMob(MobInfo* mobInfo, int playerX, int playerY, int map[][MAX_Y + MIN_Y], Player* p);
bool observeLine(int x1, int y1, int x2, int y2, int map[][MAX_Y + MIN_Y], int locationX, int locationY, int range);
void mobAttack(Player* p, MobInfo* mob);