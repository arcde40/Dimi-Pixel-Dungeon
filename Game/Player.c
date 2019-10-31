#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include <math.h>

bool attackAble(Player* p, MobInfo* info, MobList* list, LogArrayList* log) {
	if (floor(sqrt( pow((p->playerX - info->posX),2) + pow((p->playerY - info->posY),2) )) <= p->equippedWeapon->attackRange) {
		playerAttack(p, info, list, log);
	}
}

MobInfo* getMobInfoByPosition(int x, int y, MobList* list) {
	for (int i = 0; i < list->size; i++) {
		if (getMobInfo(list, i)->posX == x && getMobInfo(list, i)->posY == y) {
			return getMobInfo(list,i);
		}
	}
	return NULL;
}

void playerAttack(Player* p, MobInfo* mob, MobList* list, LogArrayList* log) {
	int damage = rand() % (p->equippedWeapon->maxDamage - p->equippedWeapon->minDamage) + p->equippedWeapon->minDamage;
	mob->Health -= damage;
	if (mob->Health <= 0) {
		for (int i = 0; i < list->size; i++) {
			if (getMobInfo(list, i)->UUID == mob->UUID) {
				removeMobInfo(list, i);
			}
		}
		char t[100] = { 0, };
		sprintf(t, "%s���� %d �������� ���� �����Ͽ���!", mob->name, damage);
		putMBS(log, t);
	}
	else {
		char t[100] = { 0, };
		sprintf(t, "%s���� %d �������� ������!", mob->name, damage);
		putMBS(log, t);
	}
}