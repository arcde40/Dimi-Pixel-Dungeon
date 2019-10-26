#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"

bool attackAble(Player* p, MobList* list, LogArrayList* log) {
	for (int i = 0; i < list->size; i++) {
		if (ABS(p->playerX - getMobInfo(list, i)->posX) + ABS(p->playerY - getMobInfo(list, i)->posY) <= p->equippedWeapon->attackRange) {
			playerAttack(p, getMobInfo(list, i), list, log);
		}
	}
	
}

MobInfo* getMobInfoByPosition(int x, int y, MobList* list) {
	for (int i = 0; i < list->size; i++) {
		if (getMobInfo(list, i)->posX == x && getMobInfo(list, i)->posY) {
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
		sprintf(t, "%s에게 %d 데미지를 입혀 제거하였다!", mob->name, damage);
		putMBS(log, t);
	}
	else {
		char t[100] = { 0, };
		sprintf(t, "%s에게 %d 데미지를 입혔다!", mob->name, damage);
		putMBS(log, t);
	}
}