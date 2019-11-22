#pragma once
#include "GlobalVariable.h"
// name, lore, id, ITEM_TYPE, level, minDamage, maxDamage, armor, attackRange, amount, attackSpeed, accuracy
typedef struct Item {
	char* name;
	char* lore;
	int id;
	int ITEM_TYPE;
	int level;
	int minDamage;
	int maxDamage;
	int armor;
	int attackRange;
	int amount;
	float attackSpeed;
	float accuarcy;
	int enhance;
	int identified;	
	int metadata;
}Item;

// Name, lore, id, ITEM_TYPE, level, minDamage, maxDamage, armor, attackRange, amount, attackSpeed, accuarcy


Item* cpyFromItemLib(int idx);
void modifyLibrary(int idx, char* name, char* lore, int id, int ITEM_TYPE, int metadata);
int getItemLibrarayIndex(int idx);