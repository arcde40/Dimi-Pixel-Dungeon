#pragma once
#include "GlobalVariable.h"
#include "MobAI.h"
#include "MobList.h"
#include "map.h"
#include <stdbool.h>


int uuidGenerator();
void setAttributeMobInfo(MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange);
void placeMob(MobInfo* info, MobList* mobList, int map[][MAX_Y + MIN_Y]);
void summonMob(MobList* mobList, int count, int difficultyOffset, int map[][MAX_Y + MIN_Y]);
