#pragma once
#include "GlobalVariable.h"
#include <stdbool.h>
#include "pointarraylist.h"
#include "LogArrayList.h"

typedef struct MobInfo {
	int UUID;
	char* name;
	int Health;
	int maxHealth;
	int minDamage;
	int maxDamage;
	int posX, posY;
	int MOB_BEHAVIOR_TYPE;
	bool isAwake;
	int sightRange;
	bool hasPath;
	PointArrayList* path;
	int index;
}MobInfo;

typedef struct MobList {
	MobInfo *data[16];
	int size;
}MobList;

MobList* initMobList();
bool isMobListEmpty(MobList* array);
void setMobInfo(MobList* arr, int idx, MobInfo* value);
void putMobInfo(MobList* arr, MobInfo* value);
void removeMobInfo(MobList* arr, int index);
MobInfo* getMobInfo(MobList* arr, int idx);