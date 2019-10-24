#pragma once
#include "MobList.h"
#include "Item.h"
#include "LogArrayList.h"

#include "GlobalVariable.h"

typedef struct Player {
	int Health, maxHealth;
	int playerX, playerY;
	Item* inventory[16];
	Item* equippedWeapon;
	Item* equippedArmor;
	Item* equippedAccesory[2];
}Player;