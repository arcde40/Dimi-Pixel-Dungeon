#include "GameManager.h"

int spawnId = 0;

int uuidGenerator() {
	//srand(time(NULL));
	return spawnId++;
}

void setAttributeMobInfo(MobInfo* info, char* name, int MOB_BEHAVIOR_TYPE, int UUID, int Health, int MaxHealth, int minDamage, int maxDamage, int sightRange) {
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
		for (int i = 0; i < component->mobList->size; i++) {
			enemyBehave(getMobInfo(component->mobList, i), component->mobList, component->player->playerX, component->player->playerY, component->map, component->player, component->player->prevPlayerX, component->player->prevPlayerY, component->logList);
		}
	}
}
void summonMob(MobList* mobList, int count, int difficultyOffset, int map[][MAX_Y+MIN_Y]) {
	for (int i = 0; i < count; i++) {
		MobInfo* info = (MobInfo*)malloc(sizeof(MobInfo));
		if (difficultyOffset == 1) {
			setAttributeMobInfo(info, "½½¶óÀÓ", MOB_BEHAVE_HOSTILE, uuidGenerator(), 40, 40, 3, 6, 2);
		}
		placeMob(info, mobList, map);
		putMobInfo(mobList, info);
	}
	
}

void spawnChest(float T1Chance, float T2Chance, float T3Chance, int ItemDropTier) {

}


