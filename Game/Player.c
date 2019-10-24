#include "Player.h"

MobInfo* attackAble(Player* p, MobList* list) {
	for (int i = 0; i < list->size; i++) {

	}
}

void playerAttack(Player* p, MobInfo* mob, MobList* list) {
	int damage = rand() % (p->equippedWeapon->maxDamage - p->equippedWeapon->minDamage) + p->equippedWeapon->minDamage;
	mob->Health -= damage;
	if (mob->Health <= 0) {
		for (int i = 0; i < list->size; i++) {
			if (getMobInfo(list, i)->UUID == mob->UUID) {
				removeMobInfo(list, i);
			}
		}
		putMBS(list, "ÀÀ");
	}
	else putMBS(list, "½È¾î");
}