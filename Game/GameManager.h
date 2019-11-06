#pragma once
#include "GlobalVariable.h"
#include "MobAI.h"
#include "MobList.h"
#include "map.h"
#include <stdbool.h>

typedef struct GameComponent{
	Player* player;
	MobList* mobList;
	RoomArrayList* roomList;
	LogArrayList* logList;
	int *cursorPosition;
	int *buttonCurPosition;
	int (*map)[MAX_Y + MIN_Y];
	WORD (*colorMap)[189];
	wchar_t (*defaultBuffer)[189];
}GameComponent;

int uuidGenerator();
void setAttributeMobInfo(MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange);
void placeMob(MobInfo* info, MobList* mobList, int map[][MAX_Y + MIN_Y]);
void nextTurn(GameComponent* component, float* actionPoint, float* cost);
void summonMob(MobList* mobList, int count, int difficultyOffset, int map[][MAX_Y + MIN_Y]);
