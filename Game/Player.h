#pragma once
#include "MobList.h"
#include "Item.h"
#include "LogArrayList.h"


#include "GlobalVariable.h"

#include <stdio.h>
typedef struct Player {
	int Health, maxHealth;
	int playerX, playerY;
	int prevPlayerX, prevPlayerY;
	Item* inventory[16];
	Item* equippedWeapon;
	Item* equippedArmor;
	Item* equippedAccesory[2];
}Player;

void playerAttack(Player* p, MobInfo* mob, MobList* list, LogArrayList* log);
bool attackAble(Player* p, MobInfo* info, MobList* list, LogArrayList* log);
MobInfo* getMobInfoByPosition(int x, int y, MobList* list);
