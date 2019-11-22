#define _CRT_SECURE_NO_WARNINGS
#include "GameManager.h"

int spawnId = 0;

// 1 - ȸ�� 2 - �� 3 - ȭ�� 4 - ���� 5 - ���� 6 - ���� 7 - ����ȭ 8 - ���� 9 - �ɾ� 10 - ��ȭ
static int potionProb[10] = { 45, 15, 10, 15, 20, 0, 15, 25, 25, 30 };
static int potionList[10] = { 1,2,3,4,5,6,7,8,9,10 };

int uuidGenerator() {
	//srand(time(NULL));
	return spawnId++;
}

void setAttributeMobInfo(int type, MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange) {
	info->type = type;
	info->name = name;
	info->MOB_BEHAVIOR_TYPE = MOB_BEHAVIOR_TYPE;
	info->UUID = UUID;
	info->Health = Health;
	info->maxHealth = MaxHealth;
	info->minDamage = minDamage;
	info->maxDamage = maxDamage;
	info->sightRange = sightRange;
	info->isAwake = false;
}

void placeMob(MobInfo* info, MobList* mobList, int map[][MAX_Y+MIN_Y]) {
	int X = 0, Y = 0;
	bool collide = false;
	while (map[X][Y] != 2 || (collide)) {
		collide = false;

		X = rand() % (MAX_X + MIN_X);
		Y = rand() % (MAX_Y + MIN_Y);

		for (int i = 0; i < mobList->size; i++) {
			if (X == getMobInfo(mobList, i)->posX && Y == getMobInfo(mobList, i)) {
				collide = true;
				break;
			}
		}

	}

	info->posX = X;
	info->posY = Y;
	
}

void nextTurn(GameComponent* component, float* actionPoint, float* cost) {
	*actionPoint -= *cost;
	for (int j = 0; *actionPoint < 1; (*actionPoint)++) {
		if (component->player->debuff[0] > 0) {
			component->player->Health += component->player->debuff[0] / 2;
			if (component->player->maxHealth < component->player->Health) component->player->Health = component->player->maxHealth;
			component->player->debuff[0] /= 2;
		}
		if (component->player->debuff[5]-- > 0) {
			component->player->Health -= component->player->debuff[5];
		}
		if (component->player->debuff[6]-- > 0) {
			component->player->Health -= 3;
		}
		if (component->player->debuff[9]-- > 0) {
			component->player->Health -= component->player->debuff[9];
		}
		for (int i = 0; i < component->mobList->size; i++) {
			enemyBehave(getMobInfo(component->mobList, i), component->mobList, component->player->playerX, component->player->playerY, component->map, component->player, component->player->prevPlayerX, component->player->prevPlayerY, component->logList);
		}
	}
}
void summonMob(MobList* mobList, int count, int difficultyOffset, int map[][MAX_Y+MIN_Y]) {
	for (int i = 0; i < count; i++) {
		MobInfo* info = (MobInfo*)malloc(sizeof(MobInfo));
		if (difficultyOffset == 1) {
			setAttributeMobInfo(1, info, "������", MOB_BEHAVE_HOSTILE, uuidGenerator(), 40, 40, 3, 6, 2);
		}
		placeMob(info, mobList, map);
		putMobInfo(mobList, info);
	}
	
}

void randomnizePotion(GameRandomnizer* rnd) {
	int shuffle[10] = { 1,2,3,4,5,6,7,8,9,10 };
	for (int i = 0; i < 100; i++) {
		int rndN = rand() % 10;
		int rndN2 = rand() % 10;
		int t = shuffle[rndN2];
		shuffle[rndN2] = shuffle[rndN];
		shuffle[rndN] = t;
	}
	memcpy(rnd->potionList, shuffle, sizeof(int) * 10);
	for (int i = 0; i < 10; i++) {
		int idx = getItemLibrarayIndex(100 + i);
		potionList[shuffle[i] - 1] = 100 + i;
		modifyLibrary(idx, NA, NA, NA, NA, (shuffle[i] | ITEM_IDENTIFIABLE | ITEM_STACKABLE | ITEM_CONSUMABLE));
	}
}

// 1Ƽ�� ���� ���� Ȯ�� : 1�� 100% 2�� 75% 3�� 25% 4�� 0%
// 2Ƽ�� ���� ���� Ȯ�� : 1�� 25% 2�� 10%
// 3Ƽ�� ���� ���� Ȯ�� : 1�� 0% (Ư�� �����θ� ����)
// ��� Ȯ���� ����������.
// 2Ƽ�� 3Ƽ�� ���ڴ� ����� �Բ�..
void spawnChest(GameComponent* cmp) {
	int count = 0;
	// 1Ƽ�� ���� ����
	while (rand() % 100 < 100 - (25 * count++)) {
		int X = 0, Y = 0;
		 do{
			X = rand() % (MAX_X + MIN_X);
			Y = rand() % (MAX_Y + MIN_Y);
		 } while (cmp->map[X][Y] != 2);
		cmp->map[X][Y] = 30; // 1Ƽ�� ���� �ڵ�
	}

	// 2Ƽ�� ���� ����
	count = 0;
	while (rand() % 100 < 25 - 15 * count) {
		int X = 0, Y = 0;
		 do{
			X = rand() % (MAX_X + MIN_X);
			Y = rand() % (MAX_Y + MIN_Y);
		 } while (cmp->map[X][Y] != 2);
		cmp->map[X][Y] = 31; // 2Ƽ�� ���� �ڵ�
		
		dropItem(cpyFromItemLib(998), cmp);
	}
}

// 1Ƽ�� ���� ���ǰ: 
// ���(50%) ����(20%) �� (10%) �׼�����(15%) ����(5%)
// �̿� ���ǰ : ��� (60%) ���� (30%) �� (10%)
// 2Ƽ�� ���� ���ǰ:
// ���� (50%) �� (15%) �׼����� (25%) ���� (10%)
// �̿� ���ǰ: ����(70%) �� (30%)
void openChest(GameComponent* cmp, int X, int Y) {
	if (cmp->map[X][Y] == 30) { // 1Ƽ�� ����
		int drops[5] = {6, 3, 1, 0, 0};
		
		switch (chances(drops, 5)) {
		case 0: dropGold(cmp, rand() % 20 + cmp->player->currentFloor * 15, X, Y);  break;
		case 1: dropItemAtPosition(dropWeapon(cmp), cmp, X, Y); break;
		case 2: dropItemAtPosition(dropArmor(cmp), cmp, X, Y); break;
		//case 3: dropItemAtPosition(dropAccesory(), cmp, X, Y); break;
		//case 4: dropItemAtPosition(dropRelic(), cmp, X, Y); break;
		}
		cmp->map[X][Y] = 2;
	}

	if (cmp->map[X][Y] == 31) {
		for (int i = 0; i < cmp->player->keyList->size; i++) {
			if (get(cmp->player->keyList, i) == 2) {
				int drops[5] = { 0, 7, 3, 0, 0 };
				switch (chances(drops, 5)) {
				case 0: dropGold(cmp, rand() % 20 + cmp->player->currentFloor * 15, X, Y);  break;
				case 1: dropItemAtPosition(dropWeapon(cmp), cmp, X, Y); break;
				case 2: dropItemAtPosition(dropArmor(cmp), cmp, X, Y); break;
				//case 3: dropItemAtPosition(dropAccesory(), cmp, X, Y); break;
					//	case 4: dropItemAtPosition(dropRelic(), cmp, X, Y); break;
				}
				cmp->map[X][Y] = 2;

				removeContent(cmp->player->keyList, i);
			}
		}
		
	}
}

int weaponChance[31] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
Item* dropWeapon(GameComponent* g) {
	Item* item1 = cpyFromItemLib(chances(weaponChance, 31));
	Item* item2 = cpyFromItemLib(chances(weaponChance, 31));
	if (ABS(g->player->level - item1->level) > ABS(g->player->level - item2->level)) {
		free(item1);
		item2->metadata |= ITEM_IDENTIFIED;
		return item2;
	}
	else {
		free(item2);
		item1->metadata |= ITEM_IDENTIFIED;
		return item1;
	}
}

Item* dropArmor(GameComponent* g) {
	Item* item1 = cpyFromItemLib(chances(weaponChance, 4) + 40);
	Item* item2 = cpyFromItemLib(chances(weaponChance, 4) + 40);
	if (ABS(g->player->level - item1->level) > ABS(g->player->level - item2->level)) {
		free(item1);
		return item2;
	}
	else {
		free(item2);
		return item1;
	}
}

Item* dropAccesory() {

}

void dropGold(GameComponent* g, int amount, int X, int Y) {
	Item* gold = cpyFromItemLib(1000);
	gold->amount = amount;
	dropItemAtPosition(gold, g, X, Y);
}

int chances(int *chances, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) sum += chances[i];
	int value = (rand() % sum);
	sum = chances[0];
	for (int i = 0; i < size; i++) {
		if (value < sum) return i;
		sum += chances[i + 1];
	}
}


// ������ ���� Ȯ��: 
// ���� ���� ���� Ȯ�� : ���� 100/(n)%
// ���� ���� Ȯ�� : 7%
// ���� ����: 
// ���� ���� - 1 : 50% ���� ���� : 48% ���� ���� + 1 : 2%
// ���� ���� : ���� * 3 - (1 ~ 3�� 3 4 ~ 5�� 2)
// �� ���� Ȯ��: 3%
// �׼����� ���� Ȯ��: 5%
// �� �����ؾ� �� ������ : ������ ����, ��ȭ�� å



void spawnItem(GameComponent* g) {
	int count = 1;
	// ���� ����
	while (rand() % 100 < 100 / ((count++))) {
		dropItem(cpyFromItemLib( potionList[chances(potionProb, 10)]), g);
	}

	// ���� ����
}


void dropItem(Item* i, GameComponent* g) {
	srand(time(NULL));
	int X = 0, Y = 0;
	bool collide = false;
	while (g->map[X][Y] != 2 || (collide)) {
		collide = false;

		X = rand() % (MAX_X + MIN_X);
		Y = rand() % (MAX_Y + MIN_Y);

		for (int i = 0; i < g->itemPos->size; i++) {
			if (X == getPoint(g->itemPos, i)->x && Y == getPoint(g->itemPos, i)->y) {
				collide = true;
				break;
			}
		}
	}
	POINT_P* p = (POINT_P*)malloc(sizeof(POINT_P));
	p->x = X; p->y = Y;
	putPoint(g->itemPos, p);
	g->itemMap[X][Y] = i;
}

void dropItemAtPosition(Item* i, GameComponent* g, int X, int Y) {
	POINT_P* p = (POINT_P*)malloc(sizeof(POINT_P));
	p->x = X; p->y = Y;
	putPoint(g->itemPos, p);
	g->itemMap[X][Y] = i;
}

// ��Ÿ������ �ؼ�
// ~ 0x1F Func Data (������ ��� �ڵ�)
// 0x20 STACKABLE - ��ø ���� ���� (False - 0, True - 1)
// 0x40 IDENTIFIABLE - �ĺ� ���� ���� (False - 0, True - 1)
// 0x80 UPGRADABLE - ��ȭ ���� ���� (False - 0, True - 1)
// 0x100 IDENTIFIED - �ĺ� ���� (False - 0, True - 1)
// 0x200 CONSUMABLE - �Ҹ� ���� (False - 0, True - 1)

void useItem(GameComponent* component, Item* i) {

	if (i->ITEM_TYPE == ITEM_POTION) {
		int potionId = i->metadata & ITEM_POTIONALLOCATED;
		// 1 - ȸ�� 2 - �� 3 - ȭ�� 4 - ���� 5 - ���� 6 - ���� 7 - ����ȭ 8 - ���� 9 - �ɾ� 10 - ��ȭ
		if (!(i->metadata & ITEM_IDENTIFIED)) {
			switch (potionId) {
			case 1: modifyLibrary(getItemLibrarayIndex(i->id), "ȸ���� ����", "������ ����� �ѵ��� �����Դϴ�. ���� �� ���Ͽ� ���� ü���� ȸ���ϰ� ��� �����̻��� ���۴ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 2: modifyLibrary(getItemLibrarayIndex(i->id), "�� ����", "ġ������ ���� ����ִ� �����Դϴ�. ���� �� �� �����̻� �ɸ��ϴ�. ��뿡�� ������ ���� ���� �� �����ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 3: modifyLibrary(getItemLibrarayIndex(i->id), "ȭ���� ����", "��� �־ ���� Ÿ�����͸� ���� �����Դϴ�. ���� �� ȭ�� �����̻� �ɸ��ϴ�. ��뿡�� ������ ���� ���� �� �����ϴ�", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 4: modifyLibrary(getItemLibrarayIndex(i->id), "������ ����", "�� ��ݸ� ���ŵ� ������ �����ϼ� ���� �Ǵ� �����Դϴ�. ���ǽ� ���� �����̻� �ɸ��ϴ�. ��뿡�� ������ ���� ���� �� �����ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 5: modifyLibrary(getItemLibrarayIndex(i->id), "������ ����", "���� ���� ���Ƽ� �̵��� �� �ֽ��ϴ�. Ÿ���� ���� �� ����� �̺�Ʈ�� �߻����� �ʽ��ϴ�. �׷��� ���� �ӵ��� �ȴ� �͸� ���� �� �����ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 6: modifyLibrary(getItemLibrarayIndex(i->id), "������ ����", "���ø� ������ 1 �����մϴ�. ������ �����Ҽ��� �� ���� ������ ���⸦ ������ �� �ֽ��ϴ�! �� ������ 1���� �����˴ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 7: modifyLibrary(getItemLibrarayIndex(i->id), "����ȭ�� ����", "���ø� ���� �������� ���� �ü��� ���� �ʰ� �˴ϴ�. ��� �ؼ� �Ҹ����� ���� �� �ִ����� �ƹ��� �𸨴ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 8: modifyLibrary(getItemLibrarayIndex(i->id), "������ ����", "��� �־ ���� �������͸� ���� �����Դϴ�. ���ǽ� ���� �����̻� �ɸ��ϴ�. ��뿡�� ������ ���� ���� �� �����ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 9: modifyLibrary(getItemLibrarayIndex(i->id), "�ɾ��� ����", "������ ����� �ѵ��� �����Դϴ�. ���� �� ���� ���� ��� ���� ��ġ�� �����ݴϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 10: modifyLibrary(getItemLibrarayIndex(i->id), "��ȭ�� ����", "��� �־ ���������°� ���� �����Դϴ�. ���ǽ� ���� �ð����� ��� ������� ���� �鿪�� �˴ϴ�.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			}
		}
		switch (potionId) {
			// ó���� �ִ� ü���� 50% ȸ��, ���� 33%�� ȸ���� ���� (10��)
		case 1: buffPlayer(component, 0, 10, component->player->maxHealth / 2); break;
		case 9: buffPlayer(component, 1, 30, NA); break;
		case 10: buffPlayer(component, 2, 15, NA); break;
		case 5: buffPlayer(component, 4, 25, NA); break;
		case 7: buffPlayer(component, 3, 20, NA); break;
			// 1 - ȸ�� 2 - �� 3 - ȭ�� 4 - ���� 5 - ���� 6 - ���� 7 - ����ȭ 8 - ���� 9 - �ɾ� 10 - ��ȭ
			// 0 ȸ�� 1 �ɾ� 2 ��ȭ 3 ����ȭ 4 ���� 5 �� 6 ȭ�� 7 ���� 8 ���� 9 ���� 10 �ұ�
		case 2: buffPlayer(component, 5, 5, NA); break;
		case 3: buffPlayer(component, 6, 5, NA); break;
		case 4: buffPlayer(component, 7, 5, NA); break;
		case 6:  break;
		case 8: buffPlayer(component, 8, 5, NA); break;
		}
		char* t[100] = { 0, };
		sprintf(t, "%s��(��) ����߽��ϴ�.", i->name);
		putMBS(component->logList, t);
			
		*component->currentLayout = 0;
		
	}
	else if (i->ITEM_TYPE == ITEM_WEAPON) {
		char t[100] = { 0, };
		sprintf(t, "%s��(��) ����߽��ϴ�.", i->name);
		putMBS(component->logList, t);
		Item* swap = component->player->equippedWeapon;
		component->player->equippedWeapon = i;
		component->player->inventory->data[*component->cursorPosition] = swap;
		*component->currentLayout = 0;

	}
	else if (i->ITEM_TYPE == ITEM_ARMOR) {
		char t[100] = { 0, };
		sprintf(t, "%s��(��) ����߽��ϴ�.", i->name);
		putMBS(component->logList, t);
		Item* swap = component->player->equippedArmor;
		component->player->equippedArmor = i;
		component->player->inventory->data[*component->cursorPosition] = swap;
		*component->currentLayout = 0;

	}

	if (i->metadata & ITEM_CONSUMABLE) {
		if (--i->amount == 0) {
			free(getItem(component->player->inventory, *component->cursorPosition));
			removeItem(component->player->inventory, *component->cursorPosition);
			if (component->cursorPosition >= component->player->inventory->size) *component->cursorPosition = component->player->inventory->size - 1;
		}
		else {
			Item* t = cpyFromItemLib(i->id);
			t->amount = i->amount;
			free(getItem(component->player->inventory, *component->cursorPosition));
			setItem(component->player->inventory, *component->cursorPosition, t);
		}
	}
}

/*
0 ȸ�� 1 �ɾ� 2 ��ȭ 3 ����ȭ 4 ���� 5 �� 6 ȭ�� 7 ���� 8 ���� 9 ���� 10 �ұ�
Metadata - ��Ÿ �Ķ���� ��Ʈ��
*/
void buffPlayer(GameComponent* component, int buffId, int duration, int metadata) {
	switch (buffId) {
	case 0:
		// Metadata�� ȸ�������� ����. ��ó���� NextTurn����
		component->player->debuff[0] = metadata; break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10: component->player->debuff[buffId] = duration; break;
	}
}

void clearBuff() {

}

void loadSaveFile(GameComponent* component) {
	FILE *fp;
	fp = fopen("save.dpd", "r");
	if (fp == NULL) {
		putMBS(component->logList, "���̺� ������ �ҷ����� ���� �����߽��ϴ�.");
		return;
	}
	char data[100] = { 0, };
	ItemArrayList* inventory = initItemArray();
	int mapLoaded[MAX_X + MIN_X][MAX_Y + MIN_Y];
	int visitMapLoaded[MAX_X + MIN_X][MAX_Y + MIN_Y];
	MobList* mobListArr= { 0, };
	Item* itemMap[MAX_X+MIN_X][MIN_Y+MAX_Y] = { 0, };
	PointArrayList* itemPos = initPointArray();
	int currentFloor;
	int playerX, playerY, Health, MaxHealth;
	//while (fscanf(fp, "%s", &data) != EOF) {
		//if (strcmp("[Player]", data)) {
			fscanf(fp,"%d, %d, %d, %d, %d", &currentFloor, &playerX, &playerY, &Health, &MaxHealth);
			int keyCount;
			fscanf(fp, "%d", &keyCount);
			component->player->keyList->size = keyCount;
			for (int i = 0; i < keyCount; i++) {
				int k;
				fscanf(fp,"%d", &k);
				component->player->keyList->data[i] = k;
			}
			// �÷��̾� ���� ������
			int wid, aid;
			fscanf(fp, "%d, %d", &wid, &aid);
			Item* w = cpyFromItemLib(wid);
			Item* a = cpyFromItemLib(aid);
			w->metadata |= ITEM_IDENTIFIED;
			a->metadata |= ITEM_IDENTIFIED;
			component->player->equippedWeapon = w;
			component->player->equippedArmor = a;

			// �÷��̾� �κ��丮
			for (int i = 0; i < 16; i++) {
				// ID, Amount, ��ȭ �ܰ�, �ĺ� ����
				int id, amount, enhance, identified, characteristic;
				fscanf(fp, "%d, %d, %d, %d, %d", &id, &amount, &enhance, &identified, &characteristic);
				if (id == 0) continue;
				Item* i = cpyFromItemLib(id);
				i->amount = amount;
				i->enhance = enhance;
				i->identified = identified;
				putItem(inventory, i);
		 	}
		//}
		//if (strcmp("[Map]", data))
		//{
			int errn;
			for (int i = 0; i < MAX_FLOOR; i++) {
				int tFloor;
				fscanf(fp, "%2d", &tFloor);
				for (int j = 0; j < MAX_X + MIN_X; j++) {
					for (int k = 0; k < MAX_Y + MIN_Y; k++) {
						errn = fscanf(fp, "%2d", &mapLoaded[j][k]);
						fscanf(fp, "%1d", &visitMapLoaded[j][k]);
					}
				}
				int mobCount ;
				fscanf(fp, "%d", &mobCount);
				mobListArr = initMobList();
				for (int l = 0; l < mobCount; l++) {
					MobInfo* TMobInfo = (MobInfo *)malloc(sizeof(MobInfo));
					int t;
					char* name = (char*)malloc(sizeof(char) * 100);
					int minDamage, maxDamage;
					fscanf(fp, "%s %d, %d, %d, %d, %d, %d, %d", name, &TMobInfo->type, &TMobInfo->Health, &TMobInfo->posX, &TMobInfo->posY, &minDamage, &maxDamage, &t);
					setAttributeMobInfo(1, TMobInfo, name, MOB_BEHAVE_HOSTILE, uuidGenerator(), TMobInfo->Health, 40, minDamage, maxDamage, 2);
					if (t == 1) TMobInfo->isAwake = true;
					putMobInfo(mobListArr, TMobInfo);
				}
				int itemCount;
				fscanf(fp, "%d", &itemCount);
				for (int i = 0; i < itemCount; i++) {
					POINT_P* point = (POINT_P*)malloc(sizeof(POINT_P));
					int id, amount, enhance, identified, metadata;
					fscanf(fp, "\n%d, %d, %d, %d, %d, %d, %d\n", &point->x, &point->y, &id, &amount, &enhance, &identified, &metadata);
					Item* i = cpyFromItemLib(id);
					i->amount = amount;
					i->enhance = enhance;
					i->identified = identified;
					i->metadata = metadata;
					itemMap[point->x][point->y] = i;
					putPoint(itemPos, point);
				}
			}
		//}
	//}
	memcpy(component->map, mapLoaded, sizeof(mapLoaded));
	//memcpy(component->visitmap, visitMapLoaded[currentFloor - 1], sizeof(visitMapLoaded[currentFloor - 1]));
	for (int i = 0; i < MAX_X + MIN_X; i++) {
		for (int j = 0; j < MAX_Y + MIN_Y; j++) {
			component->visitmap[i][j] = visitMapLoaded[i][j] == 0 ? 0 : 1;
		}
	}
	component->player->playerX = playerX;
	component->player->playerY = playerY;
	component->player->inventory = inventory;
	component->player->Health = Health;
	component->player->maxHealth = MaxHealth;
	component->mobList->size = mobListArr->size;
	for (int i = 0; i < 16; i++) {
		component->mobList->data[i] = mobListArr->data[i];
	}

	component->itemPos->size = itemPos->size;
	for (int i = 0; i < component->itemPos->size; i++) {
		component->itemPos->data[i] = itemPos->data[i];
	}

	memcpy(component->itemMap, itemMap, sizeof(Item*[MAX_X+MIN_X][MAX_Y+MIN_Y]));
	fclose(fp);
}
/*
[Player]
1, 20, 37


[Map]
1
000000000000..
5
1 20 20 30
1 30 20 30
1 20 30 40 
1 20 30 40 
1 20 20 20

*/

void writeSaveFile(GameComponent* component) {
	FILE *fp;
	fp = fopen("save.dpd", "w");
	//fprintf(fp, "[Player]\n");
	fprintf(fp, "%d, %d, %d, %d, %d\n", component->player->currentFloor, component->player->playerX, component->player->playerY, component->player->Health, component->player->maxHealth);
	fprintf(fp, "%d\n", component->player->keyList->size);
	for (int i = 0; i < component->player->keyList->size; i++) {
		fprintf(fp, "%d ", get(component->player->keyList, i));
	}
	fprintf(fp, "\n");
	fprintf(fp, "%d, %d\n", component->player->equippedWeapon->id, component->player->equippedArmor->id);
	for (int i = 0; i < 16; i++) {
		// ID, Amount, ��ȭ �ܰ�, �ĺ� ����
		Item* t = getItem(component->player->inventory, i);
		if (t == NULL) {
			fprintf(fp, "0, 0, 0, 0, 0\n");
		}
		else fprintf(fp, "%d, %d, %d, %d, %d\n", t->id, t->amount, t->enhance, t->identified, t->metadata);
		
	}
	//fprintf(fp, "[Map]\n");
	for (int i = 1; i <= MAX_FLOOR; i++) {
		fprintf(fp, "%02d", i);
		for (int j = 0; j < MAX_X + MIN_X; j++) {
			for (int k = 0; k < MAX_Y + MIN_Y; k++) {
				fprintf(fp, "%02d", component->map[j][k]);
				fprintf(fp, "%d", component->visitmap[j][k] ? 1 : 0);
			}
		}
		fprintf(fp, "\n%d", component->mobList->size);
		for (int i = 0; i < component->mobList->size; i++) {
			MobInfo* info = getMobInfo(component->mobList, i);
			fprintf(fp, "\n%s %d, %d, %d, %d, %d, %d, %d", info->name, info->type, info->Health, info->posX, info->posY, info->minDamage, info->maxDamage, info->isAwake ? 1 : 0);
		}
		fprintf(fp, "\n%d", component->itemPos->size);
		for (int i = 0; i < component->itemPos->size; i++) {
			POINT_P* point = getPoint(component->itemPos, i);
			Item* item = component->itemMap[point->x][point->y];
			fprintf(fp, "\n%d, %d, %d, %d, %d, %d, %d", point->x, point->y, item->id, item->amount, item->enhance, item->identified, item->metadata);
		}
		fclose(fp);
	}
}