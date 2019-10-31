#pragma once
// id, ITEM_TYPE, minDamage, maxDamage, attackRange, amount
typedef struct Item {
	char* name;
	char* lore;
	int id;
	int ITEM_TYPE;
	int minDamage;
	int maxDamage;
	int attackRange;
	int amount;
}Item;