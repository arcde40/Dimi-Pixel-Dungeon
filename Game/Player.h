#pragma once
#include "MobList.h"
#include "Item.h"
#include "LogArrayList.h"
#include "ItemArrayList.h"
#include "arraylist.h"

#include "GlobalVariable.h"

#include <stdio.h>
typedef struct Player {
	int Health, maxHealth;
	int playerX, playerY;
	int prevPlayerX, prevPlayerY;
	int currentFloor;
	ItemArrayList* inventory;
	Item* equippedWeapon;
	Item* equippedArmor;
	Item* equippedAccesory;
	//0 회복 1 심안 2 정화 3 투명화 4 도약 5 독 6 화염 7 마비 8 빙결 9 출혈 10 불구
	int debuff[50];
	int level;
	int money;
	ArrayList* keyList;
}Player;

typedef struct Skill {
	char* name;
	char* lore;
	int skillID;
	int special;
	int funcID;
	int paint;
}Skill;


void playerAttack(Player* p, MobInfo* mob, MobList* list, LogArrayList* log);
bool attackAble(Player* p, MobInfo* info, MobList* list, LogArrayList* log);
MobInfo* getMobInfoByPosition(int x, int y, MobList* list);
