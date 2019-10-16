#include "MobAI.h"

PointArrayList* enemyBehave(MobInfo* mobInfo, int playerX, int playerY, int map[][MAX_Y+MIN_Y], Player* p) {
	if (mobInfo->isAwake) {
		moveMob(mobInfo, playerX, playerY, map, p);
	}
	else if (mobInfo->MOB_BEHAVIOR_TYPE == MOB_BEHAVE_HOSTILE) {
		if (observeLine(playerX, playerY, mobInfo->posX, mobInfo->posY, map, playerX, playerY, mobInfo->sightRange)) {
			mobInfo->isAwake = true;
			//moveMob(mobInfo, playerX, playerY, map);
		}
	}
}

void moveMob(MobInfo* mobInfo, int playerX, int playerY, int map[][MAX_Y+MIN_Y], Player* p) {
	if ((mobInfo->posX == playerX + 1 || mobInfo->posX == playerX || mobInfo->posX == playerX - 1) && (mobInfo->posY == playerY - 1 || mobInfo->posY == playerY || mobInfo->posY == playerY + 1)) mobAttack(p, mobInfo);
	else {
		int offsetX, offsetY;
		int topX=0, topY=0, topVal = 200000;
		for (offsetX = -1; offsetX <= 1; offsetX++) {
			for (offsetY = -1; offsetY <= 1; offsetY++) {
				if (isPassable(map[mobInfo->posX + offsetX][mobInfo->posY + offsetY])) {
					if (getH(playerX, playerY, mobInfo->posX + offsetX, mobInfo->posY + offsetY) < topVal) {
						topVal = getH(playerX, playerY, mobInfo->posX + offsetX, mobInfo->posY + offsetY);
						topX = offsetX; topY = offsetY;
					}
				}
			}
		}
		mobInfo->posX += topX; mobInfo->posY += topY;
	}
}

void mobAttack(Player* p, MobInfo* mob) {
	int damage = (rand() % (mob->maxDamage - mob->minDamage)) + mob->minDamage;
	p->Health -= damage;
	return;
}


bool observeLine(int x1, int y1, int x2, int y2, int map[][MAX_Y + MIN_Y], int locationX, int locationY, int range) {
	if (ABS(y2 - y1) < ABS(x2 - x1)) {
		// Drawing ( -1 <= M <= 1 ) Line
		int dx = ABS(x2 - x1);
		int dy = ABS(y2 - y1);

		int yi = (y2 - y1 > 0) ? 1 : -1;

		int D = 2 * dy - dx;
		int y = y1;
		if (x2 - x1 < 0) {
			for (int x = x1; x > x2; x--) {
				// Process Start
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return false;
				if (ABS(x - x2) > range) return false;
				if (locationX == x && locationY == y) return true;
				if (!isTransparent(map[x][y])) return false;
				// Process End
				if (D > 0) {
					y -= yi;
					D -= dx + dx;
				}
				D += dy + dy;
			}
		}
		else {
			for (int x = x1; x < x2; x++) {
				// Process Start
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return false;
				if (ABS(x - x1) > range) return false;
				if (locationX == x && locationY == y) return true;
				if (!isTransparent(map[x][y])) return false;
				// Process End
				if (D > 0) {
					y += yi;
					D -= dx + dx;
				}
				D += dy + dy;
			}
		}
	}
	else {
		// Drawing ( M < -1  or M > 1 ) Line
		int dx = ABS(x2 - x1);
		int dy = ABS(y2 - y1);

		int xi = (x2 - x1 > 0) ? 1 : -1;

		int D = 2 * dx - dy;
		int x = x1;

		if (y2 - y1 < 0) {
			for (int y = y1; y > y2; y--) {
				// Process Start
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return false;
				if (ABS(y - y2) > range) return false;
				if (locationX == x && locationY == y) return true;
				if (!isTransparent(map[x][y])) return false;
				// Process End
				if (D > 0) {
					x -= xi;
					D -= dy + dy;
				}
				D += dx + dx;

			}
		}
		else {
			for (int y = y1; y < y2; y++) {
				// Process Start
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return false;
				if (ABS(y - y1) > range) return false;
				if (locationX == x && locationY == y) return true;
				if (!isTransparent(map[x][y])) return false;
				// Process End
				if (D > 0) {
					x += xi;
					D -= dy + dy;
				}
				D += dx + dx;

			}
		}
	}
}

/*bool drawCircleMob(int R, int playerX, int playerY, int map[][MAX_Y + MIN_Y]) {
	int x = 0, y = R;

	// Initial Process Here
	if (plotLine(playerX, playerY, playerX + x, playerY + y, map, playerX, playerY) ||
		plotLine(playerX, playerY, playerX + -x, playerY + -y, map, playerX, playerY) ||
		plotLine(playerX, playerY, playerX + y, playerY + x, map, playerX, playerY) ||
		plotLine(playerX, playerY, playerX + -y, playerY + -x, map, playerX, playerY)) return true;

	// E(first) = 2*xp + 3 = 2 * 0 + 3 = 3
	int deltaE = 3;
	// SE(first) = 2*xp - 2 *yp + 5 = 2 * 0 - 2 * R + 5 = 2 * R + 5
	int deltaSE = -2 * R + 5;

	// 판별식
	int D = 1 - R;
	while (y > x) { // 2번째 Octant까지 : 1번째 Octant로 넘어가면 y > x가 됨.
		if (D < 0) { // Midpoint가 원 안의 점 (E 선택)
			D += deltaE; // 판별식에 deltaE 증가
			deltaE += 2; // deltaE = Ep+1 - Ep = (2*(xp+1) + 3) - (2*xp + 3) = 2xp + 2 + 3 - 2*xp - 3 = 2
			deltaSE += 4; // deltaSE = SEp+1 - SEp = (2*xp+1 - 2*yp+1 + 5) - (2*xp - 2yp + 5) = 2*xp + 2 - 2*yp + 2 + 5 - 2*xp +2*yp - 5 = 4
		}
		else { // MidPoint가 원 위나 원 밖의 점 (SE 선택)
			D += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		// Process Here
		plotLine(playerX, playerY, playerX + x, playerY + y, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX - x, playerY + y, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX + x, playerY - y, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX - x, playerY - y, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX + y, playerY + x, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX + y, playerY - x, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX - y, playerY + x, colorMap, map, playerX, playerY, visitMap);
		plotLine(playerX, playerY, playerX - y, playerY - x, colorMap, map, playerX, playerY, visitMap);
	}
}*/