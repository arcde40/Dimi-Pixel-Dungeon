#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include <math.h>

Skill skills[14] = {
	{"정신 집중", "2턴간 정신 집중 상태에 돌입합니다. 모든 근접 무기 공격력이 100% 증가합니다.", 1, 0, 1, 0},
	{"가벼운 걸음", "5턴간 타일 이동에 소요되는 행동력이 절반이 됩니다. (한 턴당 2칸 이동 가능)", 2 , 0, 2, 0},
	{"돌진", "선택한 대상에게 돌진하여 무기 공격력의 50%의 피해를 입힙니다.", 3, 0, 3, 0},
	{"이단베기", "공격 범위 내의 모든 적에게 근접 무기 공격력의 120% 피해를 입힙니다.", 4, 0, 4, 0},
	{"회피 기동", "30턴간 적의 공격을 회피할 확률이 50%p 증가합니다.", 5, 0, 5, 0},
	{"발도술", "모든 근접무기의 사거리가 10턴간 2칸 증가합니다.", 6, 0, 6, 0},
	{"진풍참", "선택한 대상에게 무기 공격력의 1200% 피해를 입힙니다. 만약 대상이 이 공격으로 인해 사망한다면 자신의 공격력이 20턴간 50% 감소합니다.", 7, 0, 7, 0},
	{"정조준", "2턴간 모든 원거리 무기의 사거리가 1칸 증가하고, 공격력이 50% 증가합니다. 2턴간 움직일 수 없습니다. 스킬 공격력에 영향을 미치지 않습니다.", 8, 0, 8, 0},
	{"관통탄", "앞으로의 5번의 공격은 무조건 적중합니다.", 9, 0, 9, 0},
	{"즉결 처형", "대상의 현재 체력이 자신의 현재 체력보다 낮은 경우 대상의 최대 생명력의 100%에 달하는 공격을 합니다.", 10, 0, 10, 0},
	{"백스텝", "공격범위 내의 모든 적을 공격하며 이동 방향의 반대쪽으로 2칸 이동합니다.", 11, 0, 11, 0},
	{"속사", "대상에게 빠르게 3번의 공격을 가합니다. ", 12, 0, 12, 0},
	{"저격", "2턴간 대기한 후, 공격 범위와 상관 없이 시야 내 있는 적에게 무기 공격력의 500% 데미지를 입힙니다.", 13, 0, 13, 0},
	{"절명의 탄환", "대상에게 원거리 무기 공격력의 200% 피해를 입힙니다. 입힌 데미지의 0.5배만큼 자신의 체력을 회복합니다.", 14, 0, 14, 0},
};


bool attackAble(Player* p, MobInfo* info, MobList* list, LogArrayList* log) {
	if (ABS(p->playerX - info->posX) <= p->equippedWeapon->attackRange && ABS(p->playerY - info->posY) <= p->equippedWeapon->attackRange) {
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
		sprintf(t, "%s에게 %d 데미지를 입혀 제거하였다!", mob->name, damage);
		putMBS(log, t);
	}
	else {
		char t[100] = { 0, };
		sprintf(t, "%s에게 %d 데미지를 입혔다!", mob->name, damage);
		putMBS(log, t);
	}
}