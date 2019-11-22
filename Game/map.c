#include "map.h"

COORD startPos = { 0,0 };
COORD endPos = { 0,0 };

void draw(int map[][MIN_Y+MAX_Y], RoomArrayList* arr) {
	srand(time(NULL));
	for (int x = 0; x < arr->size; x++) {
		Room* r = getRoom(arr, x);
		for (int i = 0; i < arr->size; i++) {
			if (r->idx != i) {
				if (intersect(getRoom(arr, i), r)) {
					put(r->connected, i);
					put(getRoom(arr, i)->connected, x);
					addAll(r->connected, getRoom(arr, i)->connected);
					addAll(getRoom(arr, i)->connected, r->connected);
				}
			}
		}
		for (int i = 0; i < r->height; i++) {

			if (i == 0 || i == r->height - 1) {
				for (int j = 1; j < r->weight; j++) {

					if (map[i + r->centerX][j + r->centerY] == 1) map[i + r->centerX][j + r->centerY] = 2;
					else if (map[i + r->centerX][j + r->centerY] != 2) map[i + r->centerX][j + r->centerY] = 1;
				}
			}
			else {
				for (int j = 1; j < r->weight; j++) {
					map[i + r->centerX][j + r->centerY] = 2;
				}
			}
			if (map[i + r->centerX][r->centerY] != 2) map[i + r->centerX][r->centerY] = 1;
			if (map[i + r->centerX][r->centerY + r->weight] != 2) map[i + r->centerX][r->centerY + r->weight] = 1;

		}
	}

	for (int i = MIN_X; i < MAX_X + MIN_X - 1; i++) {
		for (int j = MIN_Y; j < MIN_Y + MAX_Y - 2; j++) {
			// Wall -> Inside conversion
			if (map[i][j - 1] == 2 && map[i][j] == 1 && map[i][j + 1] == 2) map[i][j] = 2;
			else if (map[i][j - 1] == 2 && map[i][j] == 1 && map[i][j + 1] == 1 && map[i][j + 2] == 2) {
				map[i][j] = 2;
				map[i][j + 1] = 2;
			}
			if (map[i - 1][j] == 2 && map[i][j] == 1 && map[i + 1][j] == 2) map[i][j] = 2;
			else if (map[i - 1][j] == 2 && map[i][j] == 1 && map[i + 1][j] == 1 && map[i + 2][j] == 2) {
				map[i][j] = 2;
				map[i + 1][j] = 2;
			}

			// Inside -> Wall conversion
			if (map[i][j] == 2 && (map[i - 1][j] == 0 || map[i + 1][j] == 0 || map[i][j - 1] == 0 || map[i][j + 1] == 0)) {
				map[i][j] = 1;
			}
		}
	}
	for (int i = 0; i < arr->size; i++) {
		if (getRoom(arr, i)->corridorCount > 1) continue;
		//System.out.println(i);
		Room* r;
		int tries = 0;
		do {
			int d = (int)(rand()) % arr->size;

			r = getRoom(arr, (int)(rand()) % arr->size);
			if (++tries > CORRIDOR_GENERATE_TRIES) break;
		} while ((r->corridorCount > 2 || r == getRoom(arr, i) || containsContent(r->connected, getRoom(arr, i)->idx)));
		// ** DEBUG PROPERTY **
		if (tries > CORRIDOR_GENERATE_TRIES) {
			//CorridorGenFail++;
			continue;
		}
		// ** DEBUG PROPERTY **


		r->corridorCount++;

		Room* target = getRoom(arr, i);

		// TODO Change Formula
		int startPosX = (target->centerX + target->height / 2), startPosY = (target->centerY + target->weight / 2);


		int endPosX = (r->centerX + r->height / 2), endPosY = (r->centerY + r->weight / 2);

		// Length that has minus value
		int VerticalLength = startPosY - endPosY;
		int HorizontalLength = startPosX - endPosX;

		int scopeX = startPosX, scopeY = startPosY;
		// if wall connected 3 times, adjust scope
		int offsetY = VerticalLength > 0 ? -1 : +1;
		int offsetX = HorizontalLength > 0 ? -1 : +1;

		if ((rand()) % 2 == 0) {
			for (int j = 0; j < abs(VerticalLength); j++) {
				if (map[scopeX][scopeY] == 1) map[scopeX][scopeY] = 9;
				else if (map[scopeX][scopeY] == 0) map[scopeX][scopeY] = 5;
				if (map[scopeX][scopeY] == 4) map[scopeX][scopeY] = 15;
				if (VerticalLength > 0) scopeY--; else scopeY++;
				if (map[scopeX][scopeY + offsetY] == 1 && map[scopeX][scopeY + offsetY * 2] == 1) {
					map[scopeX][scopeY] = 4; scopeX += offsetX;
				}
			}
			if (map[scopeX][scopeY] == 4 || map[scopeX][scopeY] == 5 || map[scopeX][scopeY] == 0) {
				if (VerticalLength > 0) {
					if (HorizontalLength > 0) {
						map[scopeX][scopeY] = 14;
					}
					else if (HorizontalLength < 0) {
						map[scopeX][scopeY] = 13;
					}
				}
				else if (VerticalLength < 0) {
					if (HorizontalLength > 0) {
						map[scopeX][scopeY] = 12;
					}
					else if (HorizontalLength < 0) {
						map[scopeX][scopeY] = 11;
					}
				}
			}
			for (int j = 0; j < abs(HorizontalLength); j++) {

				if (map[scopeX][scopeY] == 1) map[scopeX][scopeY] = 9;
				if (map[scopeX][scopeY] == 5) map[scopeX][scopeY] = 15;
				else if (map[scopeX][scopeY] == 0) map[scopeX][scopeY] = 4;
				if (HorizontalLength > 0) scopeX--; else scopeX++;
				if (map[scopeX + offsetX][scopeY] == 1 && map[scopeX + offsetX * 2][scopeY] == 1) {
					map[scopeX][scopeY] = 5;
					scopeY += offsetY;
				}
			}
		}
		else {
			for (int j = 0; j < abs(HorizontalLength); j++) {

				if (map[scopeX][scopeY] == 1) map[scopeX][scopeY] = 9;
				else if (map[scopeX][scopeY] == 0) map[scopeX][scopeY] = 4;
				if (map[scopeX][scopeY] == 5) map[scopeX][scopeY] = 15;
				if (HorizontalLength > 0) scopeX--; else scopeX++;
				if (map[scopeX + offsetX][scopeY] == 1 && map[scopeX + offsetX * 2][scopeY] == 1) {
					map[scopeX][scopeY] = 5; scopeY += offsetY;
				}
			}
			if (map[scopeX][scopeY] == 4 || map[scopeX][scopeY] == 5 || map[scopeX][scopeY] == 0) {
				if (HorizontalLength > 0) {
					if (VerticalLength > 0) {
						map[scopeX][scopeY] = 14;
					}
					else if (VerticalLength < 0) {
						map[scopeX][scopeY] = 13;
					}
				}
				else if (HorizontalLength < 0) {
					if (VerticalLength > 0) {
						map[scopeX][scopeY] = 12;
					}
					else if (VerticalLength < 0) {
						map[scopeX][scopeY] = 11;
					}
				}
			}
			for (int j = 0; j < abs(VerticalLength); j++) {

				if (map[scopeX][scopeY] == 1) map[scopeX][scopeY] = 9;
				else if (map[scopeX][scopeY] == 0) map[scopeX][scopeY] = 5;
				if (map[scopeX][scopeY] == 4) map[scopeX][scopeY] = 15;
				if (VerticalLength > 0) scopeY--; else scopeY++;
				if (map[scopeX][scopeY + offsetY] == 1 && map[scopeX][scopeY + offsetY * 2] == 1) {
					map[scopeX][scopeY] = 4; scopeX += offsetX;
				}
			}
		}



	}

	// Stair Generation
	// Start picks randomly, end picks least 50 distance
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	while (map[x1][y1] != 2 || map[x2][y2] != 2 || abs(x1-x2) + abs(y1-y2) < 50) {
		x1 = (int)(rand()) % (MAX_X + MIN_X);
		x2 = (int)(rand()) % (MAX_X + MIN_X);
		y1 = (int)(rand()) % (MAX_Y + MIN_Y);
		y2 = (int)(rand()) % (MAX_Y + MIN_Y);
	}
	startPos.X = x1;
	startPos.Y = y1;
	endPos.X = x2;
	endPos.Y = y2;
	map[x1][y1] = 21;
	map[x2][y2] = 22;
}

bool isPassable(int tile) {
	if (tile == 2 || tile == 3 || tile == 4 || tile == 5 || tile == 9 || tile == 11 || tile == 12 || tile == 13 || tile == 14 || tile == 15 || tile == 21 || tile == 22) return true;
	return false;
}

bool isOverlapping(int x, int y, MobList* mobList) {
	for (int i = 0; i < mobList->size; i++) {
		if (getMobInfo(mobList, i)->posX == x && getMobInfo(mobList, i)->posY == y) return true;
	}
	return false;
}

bool isTransparent(int tile) {
	if (tile == 2 || tile == 3 || tile == 4 || tile == 5 || tile == 11 || tile == 12 || tile == 13 || tile == 14 || tile == 15 || tile == 21 || tile == 22 || tile == 30 || tile == 31) return true;
	return false;
}

COORD getStartPos() {
	return startPos;
}

COORD getEndPos() {
	return endPos;
}

/* Wall Variation
*  0x10 - Wall
*  + 0x11
*  
*/

void BitMapMapper(int map[][MIN_Y + MAX_Y], int BitMapMapping[][MIN_Y + MAX_Y]) {
	for (int i = MIN_X; i < MAX_X + MIN_X; i++) {
		for (int j = MIN_Y; j < MIN_Y + MAX_Y; j++) {
			if (map[i][j] == 1 && (map[i - 1][j] == 1 || map[i - 1][j] == 9) && (map[i + 1][j] == 1 || map[i + 1][j] == 9) && (map[i][j + 1] == 1 || map[i][j + 1] == 9) && (map[i][j - 1] == 1 || map[i][j - 1] == 9)) {
				BitMapMapping[i][j] = 0x11; // +
			}
			else if (map[i][j] == 1 && (map[i - 1][j] != 1 && map[i - 1][j] != 9) && (map[i + 1][j] == 1 || map[i + 1][j] == 9) && (map[i][j + 1] == 1 || map[i][j + 1] == 9) && (map[i][j - 1] != 1 && map[i][j - 1] != 9)) {
				BitMapMapping[i][j] = 0x12; // r
			}
			else if (map[i][j] == 1 && (map[i - 1][j] != 1 && map[i - 1][j] != 9) && (map[i + 1][j] != 1 && map[i + 1][j] != 9) && (map[i][j + 1] == 1 || map[i][j + 1] == 9) && (map[i][j - 1] == 1 || map[i][j - 1] == 9)) {
				BitMapMapping[i][j] = 0x13; // -
			}
			else if (map[i][j] == 1 && (map[i - 1][j] != 1 && map[i - 1][j] != 9) && (map[i + 1][j] == 1 || map[i + 1][j] == 9) && (map[i][j + 1] != 1 && map[i][j + 1] != 9) && (map[i][j - 1] == 1 || map[i][j - 1] == 9)) {
				BitMapMapping[i][j] = 0x14; // ¤¡
			}
			else if (map[i][j] == 1 && (map[i - 1][j] == 1 || map[i - 1][j] == 9) && (map[i + 1][j] == 1 || map[i + 1][j] == 9) && (map[i][j + 1] != 1 && map[i][j + 1] != 9) && (map[i][j - 1] != 1 && map[i][j - 1] != 9)) {
				BitMapMapping[i][j] = 0x15; // l
			}
			else if (map[i][j] == 1 && (map[i - 1][j] == 1 || map[i - 1][j] == 9) && (map[i + 1][j] != 1 && map[i + 1][j] != 9) && (map[i][j + 1] == 1 || map[i][j + 1] == 9) && (map[i][j - 1] != 1 && map[i][j - 1] != 9)) {
				BitMapMapping[i][j] = 0x16; // ¤¤
			}
			else if (map[i][j] == 1 && (map[i - 1][j] == 1 || map[i - 1][j] == 9) && (map[i + 1][j] != 1 && map[i + 1][j] != 9) && (map[i][j + 1] != 1 && map[i][j + 1] != 9) && (map[i][j - 1] == 1 || map[i][j - 1] == 9)) {
				BitMapMapping[i][j] = 0x17; // j
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] == 2 && map[i][j + 1] == 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x101; // ¤±
			}
			else if (map[i][j] == 2 && map[i - 1][j] != 2 && map[i + 1][j] == 2 && map[i][j + 1] == 2 && map[i][j - 1] != 2) {
				BitMapMapping[i][j] = 0x102; // r
			}
			else if (map[i][j] == 2 && map[i - 1][j] != 2 && map[i + 1][j] == 2 && map[i][j + 1] != 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x103; // ¤¡
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] != 2 && map[i][j + 1] != 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x104; // j
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] != 2 && map[i][j + 1] == 2 && map[i][j - 1] != 2) {
				BitMapMapping[i][j] = 0x105; // ¤¤
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] == 2 && map[i][j + 1] != 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x106; // ¤Ó
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] == 2 && map[i][j + 1] == 2 && map[i][j - 1] != 2) {
				BitMapMapping[i][j] = 0x107; // l
			}
			else if (map[i][j] == 2 && map[i - 1][j] == 2 && map[i + 1][j] != 2 && map[i][j + 1] == 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x108; // _
			}
			else if (map[i][j] == 2 && map[i - 1][j] != 2 && map[i + 1][j] == 2 && map[i][j + 1] == 2 && map[i][j - 1] == 2) {
				BitMapMapping[i][j] = 0x109; // -
			}
			else if (map[i][j] == 9 && map[i-1][j] == 1 && map[i+1][j] == 1) {
				BitMapMapping[i][j] = 0x201; // Horizontal Door
			}
			else if (map[i][j] == 9 && map[i][j-1] == 1 && map[i][j+1] == 1) {
				BitMapMapping[i][j] = 0x202; // Vertical Door
			}
			else if (map[i][j] == 21) {
				BitMapMapping[i][j] = 0x300;
			}
			else if (map[i][j] == 22) {
				BitMapMapping[i][j] = 0x301;
			}
			else if (map[i][j] == 4) {
				BitMapMapping[i][j] = 0x401;
			}
			else if (map[i][j] == 5) {
				BitMapMapping[i][j] = 0x402;
			}
			else if (map[i][j] == 11) {
				BitMapMapping[i][j] = 0x403;
			}
			else if (map[i][j] == 12) {
				BitMapMapping[i][j] = 0x404;
			}
			else if (map[i][j] == 13) {
				BitMapMapping[i][j] = 0x405;
			}
			else if (map[i][j] == 14) {
				BitMapMapping[i][j] = 0x406;
			}
			else if (map[i][j] == 15) {
				BitMapMapping[i][j] = 0x407;
			}
		}
	}
}