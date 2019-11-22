#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include <math.h>

Skill skills[14] = {
	{"���� ����", "2�ϰ� ���� ���� ���¿� �����մϴ�. ��� ���� ���� ���ݷ��� 100% �����մϴ�.", 1, 0, 1, 0},
	{"������ ����", "5�ϰ� Ÿ�� �̵��� �ҿ�Ǵ� �ൿ���� ������ �˴ϴ�. (�� �ϴ� 2ĭ �̵� ����)", 2 , 0, 2, 0},
	{"����", "������ ��󿡰� �����Ͽ� ���� ���ݷ��� 50%�� ���ظ� �����ϴ�.", 3, 0, 3, 0},
	{"�̴ܺ���", "���� ���� ���� ��� ������ ���� ���� ���ݷ��� 120% ���ظ� �����ϴ�.", 4, 0, 4, 0},
	{"ȸ�� �⵿", "30�ϰ� ���� ������ ȸ���� Ȯ���� 50%p �����մϴ�.", 5, 0, 5, 0},
	{"�ߵ���", "��� ���������� ��Ÿ��� 10�ϰ� 2ĭ �����մϴ�.", 6, 0, 6, 0},
	{"��ǳ��", "������ ��󿡰� ���� ���ݷ��� 1200% ���ظ� �����ϴ�. ���� ����� �� �������� ���� ����Ѵٸ� �ڽ��� ���ݷ��� 20�ϰ� 50% �����մϴ�.", 7, 0, 7, 0},
	{"������", "2�ϰ� ��� ���Ÿ� ������ ��Ÿ��� 1ĭ �����ϰ�, ���ݷ��� 50% �����մϴ�. 2�ϰ� ������ �� �����ϴ�. ��ų ���ݷ¿� ������ ��ġ�� �ʽ��ϴ�.", 8, 0, 8, 0},
	{"����ź", "�������� 5���� ������ ������ �����մϴ�.", 9, 0, 9, 0},
	{"��� ó��", "����� ���� ü���� �ڽ��� ���� ü�º��� ���� ��� ����� �ִ� ������� 100%�� ���ϴ� ������ �մϴ�.", 10, 0, 10, 0},
	{"�齺��", "���ݹ��� ���� ��� ���� �����ϸ� �̵� ������ �ݴ������� 2ĭ �̵��մϴ�.", 11, 0, 11, 0},
	{"�ӻ�", "��󿡰� ������ 3���� ������ ���մϴ�. ", 12, 0, 12, 0},
	{"����", "2�ϰ� ����� ��, ���� ������ ��� ���� �þ� �� �ִ� ������ ���� ���ݷ��� 500% �������� �����ϴ�.", 13, 0, 13, 0},
	{"������ źȯ", "��󿡰� ���Ÿ� ���� ���ݷ��� 200% ���ظ� �����ϴ�. ���� �������� 0.5�踸ŭ �ڽ��� ü���� ȸ���մϴ�.", 14, 0, 14, 0},
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
		sprintf(t, "%s���� %d �������� ���� �����Ͽ���!", mob->name, damage);
		putMBS(log, t);
	}
	else {
		char t[100] = { 0, };
		sprintf(t, "%s���� %d �������� ������!", mob->name, damage);
		putMBS(log, t);
	}
}