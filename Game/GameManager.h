#pragma once
#include "GlobalVariable.h"
#include "MobAI.h"
#include "MobList.h"
#include "map.h"
#include <stdbool.h>
#include <string.h>
#include "ItemArrayList.h"
#include "arraylist.h"

typedef struct GameComponent{
	Player* player;
	MobList* mobList;
	RoomArrayList* roomList;
	LogArrayList* logList;
	int *cursorPosition;
	int *buttonCurPosition;
	int *currentLayout;
	int *gameState;
	int (*map)[MAX_Y + MIN_Y];
	bool (*visitmap)[MAX_Y + MIN_Y];
	WORD (*colorMap)[189];
	wchar_t (*defaultBuffer)[189];
	Item* (*itemMap)[MAP_Y_SIZE];
	PointArrayList* itemPos;
}GameComponent;

typedef struct GameRandomnizer {
	int potionList[10];
	int seed;
}GameRandomnizer;

int uuidGenerator();
void setAttributeMobInfo(int type, MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange, int accuracy, int dodge);
void placeMob(MobInfo* info, MobList* mobList, int map[][MAX_Y + MIN_Y]);
void nextTurn(GameComponent* component, float* actionPoint, float* cost);
void summonMob(MobList* mobList, int count, int difficultyOffset, int map[][MAX_Y + MIN_Y]);
void writeSaveFile(GameComponent* component);
void loadSaveFile(GameComponent* component);
void dropItem(Item* i, GameComponent* g);
void spawnItem(GameComponent* g);
int chances(int *chances, int size);
void useItem(GameComponent* component, Item* i);
void buffPlayer(GameComponent* component, int buffId, int duration, int metadata);
void randomnizePotion(GameRandomnizer* rnd);
Item* dropWeapon(GameComponent* component);
Item* dropArmor(GameComponent* component);
Item* dropAccesory();
void openChest(GameComponent* cmp, int X, int Y);
void spawnChest(GameComponent* cmp);
void dropItemAtPosition(Item* i, GameComponent* g, int X, int Y);
void dropGold(GameComponent* g, int amount, int X, int Y);