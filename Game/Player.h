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
	//0 ȸ�� 1 �ɾ� 2 ��ȭ 3 ����ȭ 4 ���� 5 �� 6 ȭ�� 7 ���� 8 ���� 9 ���� 10 �ұ�
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
