#define _CRT_SECURE_NO_WARNINGS
#include "GameManager.h"

int spawnId = 0;

// 1 - 회복 2 - 독 3 - 화염 4 - 마비 5 - 도약 6 - 지식 7 - 투명화 8 - 빙결 9 - 심안 10 - 정화
static int potionProb[10] = { 45, 15, 10, 15, 20, 0, 15, 25, 25, 30 };
static int potionList[10] = { 1,2,3,4,5,6,7,8,9,10 };


// 몬스터의 UUID를 만들어주는 함수.
int uuidGenerator() {
	//srand(time(NULL));
	return spawnId++;
}

// MobInfo의 정보를 한번에 바꿔주는 함수.
void setAttributeMobInfo(int type, MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange, int accuracy, int dodge) {
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
	info->accuracyConstant = accuracy;
	info->dodgeConstant = dodge;
}

// MobInfo에 있는 정보를 토대로 실제 맵상에 놓아주는 함수. 생성될 수 있는 랜덤한 위치에 놓아줍니다!
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

// 다음턴으로 넘어갑니다.
void nextTurn(GameComponent* component, float* actionPoint, float* cost) {
	*actionPoint -= *cost;
	for (int j = 0; *actionPoint < 1 || component->player->debuff[7] >= 0; (*actionPoint)++) {
		if (component->player->debuff[7] > 0) (*actionPoint)--;
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
		component->player->debuff[3]--;
		component->player->debuff[1]--;
		component->player->debuff[2]--;
		component->player->debuff[8]--;
		for (int i = 0; i < component->mobList->size; i++) {
			if(component->player->debuff[3] < 0) enemyBehave(getMobInfo(component->mobList, i), component->mobList, component->player->playerX, component->player->playerY, component->map, component->player, component->player->prevPlayerX, component->player->prevPlayerY, component->logList);
		}
	}
}
void summonMob(MobList* mobList, int count, int offset, int map[][MAX_Y+MIN_Y]) {
	for (int i = 0; i < count; i++) {
		MobInfo* info = (MobInfo*)malloc(sizeof(MobInfo));
		int chance[6] = {10, 6, 1, 3, 3, 2};
		int difficultyOffset = chances(chance, 6) + 1;
		if (difficultyOffset == 1) {
			setAttributeMobInfo(1, info, "슬라임", MOB_BEHAVE_HOSTILE, uuidGenerator(), 10, 10, 3, 6, 2, 5, 1);
		}
		if (difficultyOffset == 2) {
			setAttributeMobInfo(2, info, "좀비", MOB_BEHAVE_HOSTILE, uuidGenerator(), 20, 20, 4, 7, 3, 1, 1);
		}
		if (difficultyOffset == 3) {
			setAttributeMobInfo(3, info, "스켈레톤", MOB_BEHAVE_RANGED, uuidGenerator(), 15, 15, 5, 8, 10, 7, 1);
		}
		if (difficultyOffset == 4) {
			setAttributeMobInfo(4, info, "좀벌레", MOB_BEHAVE_HOSTILE, uuidGenerator(), 5, 5, 9, 13, 3, 3, 3);
		}
		if (difficultyOffset == 5) {
			setAttributeMobInfo(5, info, "거미", MOB_BEHAVE_HOSTILE, uuidGenerator(), 20, 20, 4, 7, 3, 1, 5);
		}
		if (difficultyOffset == 6) {
			setAttributeMobInfo(6, info, "블레이즈", MOB_BEHAVE_HOSTILE, uuidGenerator(), 12, 12, 9, 18, 3, 1, 1);
		}
		if (difficultyOffset == 10) {
			setAttributeMobInfo(10, info, "???", MOB_BEHAVE_HOSTILE, uuidGenerator(), 48, 48, 2, 17, 10, 10, 0);
		}
		if (difficultyOffset == 11) {
			setAttributeMobInfo(11, info, "???", MOB_BEHAVE_HOSTILE, uuidGenerator(), 100, 100, 5, 24, 10, 12, 2);
		}
		
		info->maxDamage += offset;
		info->minDamage += offset + 1;
		info->maxHealth *= offset * offset / 4 + offset * 3 / 4 + (3 / 2);
		info->Health = info->maxHealth;
		placeMob(info, mobList, map);
		putMobInfo(mobList, info);
	}
	
}

// 포션의 효과를 랜덤 배열합니다.
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

// 상자를 생성합니다.

// 1티어 상자 나올 확률 : 1개 100% 2개 75% 3개 25% 4개 0%
// 2티어 상자 나올 확률 : 1개 25% 2개 10%
// 3티어 상자 나올 확률 : 1개 0% (특수 방으로만 생성)
// 모든 확률은 독립시행임.
// 2티어 3티어 상자는 열쇠와 함께..
void spawnChest(GameComponent* cmp) {
	int count = 0;
	// 1티어 상자 생성
	while (rand() % 100 < 100 - (25 * count++)) {
		int X = 0, Y = 0;
		 do{
			X = rand() % (MAX_X + MIN_X);
			Y = rand() % (MAX_Y + MIN_Y);
		 } while (cmp->map[X][Y] != 2);
		cmp->map[X][Y] = 30; // 1티어 상자 코드
	}

	// 2티어 상자 생성
	count = 0;
	while (rand() % 100 < 25 - 15 * count) {
		int X = 0, Y = 0;
		 do{
			X = rand() % (MAX_X + MIN_X);
			Y = rand() % (MAX_Y + MIN_Y);
		 } while (cmp->map[X][Y] != 2);
		cmp->map[X][Y] = 31; // 2티어 상자 코드
		
		dropItem(cpyFromItemLib(998), cmp);
	}
}

// XY위치에 있는 상자를 엽니다.

// 1티어 상자 드랍품: 
// 골드(50%) 무기(20%) 방어구 (10%) 액서서리(15%) 유물(5%)
// 미완 드랍품 : 골드 (60%) 무기 (30%) 방어구 (10%)
// 2티어 상자 드랍품:
// 무기 (50%) 방어구 (15%) 액서서리 (25%) 유물 (10%)
// 미완 드랍품: 무기(70%) 방어구 (30%)
void openChest(GameComponent* cmp, int X, int Y) {
	if (cmp->map[X][Y] == 30) { // 1티어 상자
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

int weaponChance[31] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
// 무기를 드랍합니다.
Item* dropWeapon(GameComponent* g) {
	Item* item1 = cpyFromItemLib(chances(weaponChance, 31) + 1);
	Item* item2 = cpyFromItemLib(chances(weaponChance, 31) + 1);
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

// 방어구를 드랍합니다.
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

// 미구현 ㅠㅠ
Item* dropAccesory() {

}
// 골드를 드랍합니다. 골드는 아직 아무 의미 없습니다.
void dropGold(GameComponent* g, int amount, int X, int Y) {
	Item* gold = cpyFromItemLib(1000);
	gold->amount = amount;
	dropItemAtPosition(gold, g, X, Y);
}

// 확률 배열 중 임의로 하나 뽑습니다.
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


// 아이템 스폰 확률: 
// 랜덤 물약 스폰 확률 : 개당 100/(n)%
// 무기 스폰 확률 : 7%
// 무기 레벨: 
// 적정 레벨 - 1 : 50% 적정 레벨 : 48% 적정 레벨 + 1 : 2%
// 적정 레벨 : 계층 * 3 - (1 ~ 3층 3 4 ~ 5층 2)
// 방어구 스폰 확률: 3%
// 액서서리 스폰 확률: 5%
// 꼭 스폰해야 할 아이템 : 지식의 물약, 강화의 책


// 아이템을 랜덤으로 드랍합니다.
void spawnItem(GameComponent* g) {
	int count = 1;
	// 물약 스폰
	while (rand() % 100 < 100 / ((count++))) {
		dropItem(cpyFromItemLib( potionList[chances(potionProb, 10)]), g);
	}

	// 무기 스폰
}

// 아이템을 드롭합니다. 랜덤으로 생성됩니다!
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

// 특정 위치에 아이템을 드랍합니다.
void dropItemAtPosition(Item* i, GameComponent* g, int X, int Y) {
	POINT_P* p = (POINT_P*)malloc(sizeof(POINT_P));
	p->x = X; p->y = Y;
	putPoint(g->itemPos, p);
	g->itemMap[X][Y] = i;
}

// 메타데이터 해석
// ~ 0x1F Func Data (아이템 펑션 코드)
// 0x20 STACKABLE - 중첩 가능 여부 (False - 0, True - 1)
// 0x40 IDENTIFIABLE - 식별 가능 여부 (False - 0, True - 1)
// 0x80 UPGRADABLE - 강화 가능 여부 (False - 0, True - 1)
// 0x100 IDENTIFIED - 식별 여부 (False - 0, True - 1)
// 0x200 CONSUMABLE - 소모성 여부 (False - 0, True - 1)

// 아이템을 장착/사용합니다.
void useItem(GameComponent* component, Item* i) {

	if (i->ITEM_TYPE == ITEM_POTION) {
		int potionId = i->metadata & ITEM_POTIONALLOCATED;
		// 1 - 회복 2 - 독 3 - 화염 4 - 마비 5 - 도약 6 - 지식 7 - 투명화 8 - 빙결 9 - 심안 10 - 정화
		if (!(i->metadata & ITEM_IDENTIFIED)) {
			switch (potionId) {
			case 1: modifyLibrary(getItemLibrarayIndex(i->id), "회복의 물약", "은은한 기운이 겉도는 물약입니다. 마실 시 몇턴에 걸쳐 체력을 회복하고 모든 상태이상을 없앱니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 2: modifyLibrary(getItemLibrarayIndex(i->id), "독 물약", "치명적인 독이 녹아있는 물약입니다. 마실 시 독 상태이상에 걸립니다. 상대에게 던지는 편이 좋을 것 같습니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 3: modifyLibrary(getItemLibrarayIndex(i->id), "화염의 물약", "들고만 있어도 손이 타버릴것만 같은 물약입니다. 마실 시 화상 상태이상에 걸립니다. 상대에게 던지는 편이 좋을 것 같습니다", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 4: modifyLibrary(getItemLibrarayIndex(i->id), "마비의 물약", "한 모금만 마셔도 사지를 움직일수 없게 되는 물약입니다. 마실시 마비 상태이상에 걸립니다. 상대에게 던지는 편이 좋을 것 같습니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 5: modifyLibrary(getItemLibrarayIndex(i->id), "도약의 물약", "지면 위를 날아서 이동할 수 있습니다. 타일을 밟을 때 생기는 이벤트가 발생하지 않습니다. 그러나 나는 속도가 걷는 것만 못한 것 같습니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 6: modifyLibrary(getItemLibrarayIndex(i->id), "지식의 물약", "마시면 지식이 1 증가합니다. 지식이 증가할수록 더 높은 레벨의 무기를 장착할 수 있습니다! 한 계층당 1개만 생성됩니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 7: modifyLibrary(getItemLibrarayIndex(i->id), "투명화의 물약", "마시면 몸이 투명해져 적의 시선을 끌지 않게 됩니다. 어떻게 해서 소리까지 없앨 수 있는지는 아무도 모릅니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 8: modifyLibrary(getItemLibrarayIndex(i->id), "빙결의 물약", "들고만 있어도 손이 얼어버릴것만 같은 물약입니다. 마실시 빙결 상태이상에 걸립니다. 상대에게 던지는 편이 좋을 것 같습니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 9: modifyLibrary(getItemLibrarayIndex(i->id), "심안의 물약", "따뜻한 기운이 겉도는 물약입니다. 마실 시 현재 층의 모든 적의 위치를 보여줍니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			case 10: modifyLibrary(getItemLibrarayIndex(i->id), "정화의 물약", "들고만 있어도 안전해지는것 같은 물약입니다. 마실시 일정 시간동안 모든 디버프에 대해 면역이 됩니다.", NA, NA, i->metadata | ITEM_IDENTIFIED); break;
			}
		}
		switch (potionId) {
			// 처음은 최대 체력의 50% 회복, 이후 33%씩 회복량 감소 (10턴)
		case 1: buffPlayer(component, 0, 10, component->player->maxHealth / 2); break;
		case 9: buffPlayer(component, 1, 30, NA); break;
		case 10: buffPlayer(component, 2, 15, NA); break;
		case 5: buffPlayer(component, 4, 25, NA); break;
		case 7: buffPlayer(component, 3, 20, NA); break;
			// 1 - 회복 2 - 독 3 - 화염 4 - 마비 5 - 도약 6 - 지식 7 - 투명화 8 - 빙결 9 - 심안 10 - 정화
			// 0 회복 1 심안 2 정화 3 투명화 4 도약 5 독 6 화염 7 마비 8 빙결 9 출혈 10 불구
		case 2: buffPlayer(component, 5, 5, NA); break;
		case 3: buffPlayer(component, 6, 5, NA); break;
		case 4: buffPlayer(component, 7, 5, NA); break;
		case 6:  break;
		case 8: buffPlayer(component, 8, 5, NA); break;
		}
		char* t[100] = { 0, };
		sprintf(t, "%s을(를) 사용했습니다.", i->name);
		putMBS(component->logList, t);
		mciSendString("play potion.wav", NULL, 0, 0);
		*component->currentLayout = 0;
		
	}
	else if (i->ITEM_TYPE == ITEM_WEAPON) {
		char t[100] = { 0, };
		sprintf(t, "%s을(를) 장비했습니다.", i->name);
		putMBS(component->logList, t);
		Item* swap = component->player->equippedWeapon;
		component->player->equippedWeapon = i;
		component->player->inventory->data[*component->cursorPosition] = swap;
		*component->currentLayout = 0;

	}
	else if (i->ITEM_TYPE == ITEM_ARMOR) {
		char t[100] = { 0, };
		sprintf(t, "%s을(를) 장비했습니다.", i->name);
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
0 회복 1 심안 2 정화 3 투명화 4 도약 5 독 6 화염 7 마비 8 빙결 9 출혈 10 불구
Metadata - 기타 파라미터 비트열
*/
// 플레이어에게 버프/디버프를 부여합니다.
void buffPlayer(GameComponent* component, int buffId, int duration, int metadata) {
	switch (buffId) {
	case 0:
		// Metadata가 회복량으로 쓰임. 후처리는 NextTurn에서
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
// 미구현
void clearBuff() {

}
// 세이브파일을 로드합니다.
void loadSaveFile(GameComponent* component) {
	FILE *fp;
	fp = fopen("save.dpd", "r");
	if (fp == NULL) {
		putMBS(component->logList, "세이브 파일을 불러오는 데에 실패했습니다.");
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
	int level, exp, acc, dod;
	//while (fscanf(fp, "%s", &data) != EOF) {
		//if (strcmp("[Player]", data)) {

	//component->player = (Player*)malloc(sizeof(Player));
	component->player->keyList = initArrayList();
	component->gameState = 1;
			fscanf(fp,"%d, %d, %d, %d, %d, %d, %d, %d, %d", &currentFloor, &playerX, &playerY, &Health, &MaxHealth, &level, &exp, &acc, &dod);
			component->player->Health = Health;
			component->player->maxHealth = MaxHealth;
			component->player->playerX = playerX;
			component->player->playerY = playerY;
			component->player->level = level;
			component->player->exp = exp;
			component->player->accuracyConstant = acc;
			component->player->dodgeConstant = dod;
			memset(component->player->debuff, 0, sizeof(component->player->debuff));
			int keyCount;
			fscanf(fp, "%d", &keyCount);
			component->player->keyList->size = keyCount;
			for (int i = 0; i < keyCount; i++) {
				int k;
				fscanf(fp,"%d", &k);
				component->player->keyList->data[i] = k;
			}
			// 플레이어 장착 아이템
			int wid, aid;
			fscanf(fp, "%d, %d", &wid, &aid);
			Item* w = cpyFromItemLib(wid);
			Item* a = cpyFromItemLib(aid);
			w->metadata |= ITEM_IDENTIFIED;
			a->metadata |= ITEM_IDENTIFIED;
			component->player->equippedWeapon = w;
			component->player->equippedArmor = a;

			// 플레이어 인벤토리
			for (int i = 0; i < 16; i++) {
				// ID, Amount, 강화 단계, 식별 여부
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
					fscanf(fp, "%s %d, %d, %d, %d, %d, %d, %d, %d, %d", name, &TMobInfo->type, &TMobInfo->Health, &TMobInfo->posX, &TMobInfo->posY, &minDamage, &maxDamage, &t, &TMobInfo->accuracyConstant, &TMobInfo->dodgeConstant);
					setAttributeMobInfo(1, TMobInfo, name, MOB_BEHAVE_HOSTILE, uuidGenerator(), TMobInfo->Health, 40, minDamage, maxDamage, 2, TMobInfo->accuracyConstant, TMobInfo->dodgeConstant);
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
// 세이브파일을 저장합니다.
void writeSaveFile(GameComponent* component) {
	FILE *fp;
	fp = fopen("save.dpd", "w");
	//fprintf(fp, "[Player]\n");
	fprintf(fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d\n", component->player->currentFloor, component->player->playerX, component->player->playerY, component->player->Health, component->player->maxHealth, component->player->level, component->player->exp, component->player->accuracyConstant, component->player->dodgeConstant);
	fprintf(fp, "%d\n", component->player->keyList->size);
	for (int i = 0; i < component->player->keyList->size; i++) {
		fprintf(fp, "%d ", get(component->player->keyList, i));
	}
	fprintf(fp, "\n");
	fprintf(fp, "%d, %d\n", component->player->equippedWeapon->id, component->player->equippedArmor->id);
	for (int i = 0; i < 16; i++) {
		// ID, Amount, 강화 단계, 식별 여부
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
			fprintf(fp, "\n%s %d, %d, %d, %d, %d, %d, %d, %d, %d", info->name, info->type, info->Health, info->posX, info->posY, info->minDamage, info->maxDamage, info->isAwake ? 1 : 0, info->accuracyConstant, info->dodgeConstant);
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