#include "lighting.h"

void defaultLighting(WORD colorMap[][189]) {
	for (int i = 0; i < 49; i++) {
		for (int j = 0; j < 189; j++) {
			colorMap[i][j] = COLOR_BRIGHT_WHITE;
		}
	}
}

void mapLighting(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y+MIN_Y], bool visitMap[][MAX_Y+MIN_Y]) {
	for (int i = MAP_X_START; i < MAP_X_END; i++) {
		for (int j = MAP_Y_START; j < MAP_Y_END-1; j++) {
			if (sqaure(i-25) + sqaure(j-66) <= sqaure(renderRange)) {
				visitMap[playerX + i - 25][playerY + j - 66] = true;
				colorMap[i][j] = COLOR_BRIGHT_WHITE;
			}
			else {
				if (visitMap[playerX + i - 25][playerY + j - 66]) {
					colorMap[i][j] = COLOR_BRIGHT_BLACK;
				}
				else {
					colorMap[i][j] = COLOR_BLACK;
				}
			}
		}
	}
}

void applyColor(WORD* colorMap, wchar_t* buffer, wchar_t* appliedBuffer) {
	for (int i = 0; i < 189 * 5; i++) appliedBuffer[i] = 0;
	int prev = -1, scope = 0;
	for (int i = 0; i < 189; i++) {
		if (prev != colorMap[i]) {
			prev = colorMap[i];
			appliedBuffer[scope++] = '\x1b';
			appliedBuffer[scope++] = '[';
			appliedBuffer[scope++] = colorMap[i] / 10 + '0';
			appliedBuffer[scope++] = colorMap[i] % 10 + '0';
			appliedBuffer[scope++] = 'm';
			appliedBuffer[scope++] = buffer[i];
		}
		else {
			appliedBuffer[scope++] = buffer[i];
		}
	}
}

/* 라이팅 기법
* 플레이어 주위로 원을 그림
* 플레이어 좌표 (x1,y2)와 원 위의 점 (xi, yi)를 잇는 직선을 그림
* 직선을 따라 색상을 변환하다가 장애물을 만나면 탐색 종료.
*/

/*
	실제 주소: 콘솔창의 절대 좌표
	가상 주소: 맵상의 절대 좌표

	실제 주소(25,66)에 가상 주소 (플레이어 x값, 플레이어 y값)이 대응됨. 따라서
	가상주소 = (플레이어 x값 + 실제 주소의 x값 - 25, 플레이어 y값 + 실제 주소의 y값 - 66)
	실제주소 = (가상주소 x값 + 25 - 플레이어 x값, 가상주소 y값 + 66 - 플레이어 y값)


*/

void mapLighting1(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y+MIN_Y], bool visitMap[][MAX_Y + MIN_Y]) {
	defaultLighting(colorMap);
	drawCircle(renderRange, playerX, playerY, colorMap, map, visitMap);
	for (int i = MAP_X_START; i < MAP_X_END; i++) {
		for (int j = MAP_Y_START; j < MAP_Y_END; j++) {
			if ( colorMap[i][j] != COLOR_CHECKED){// colorMap[i][j] != COLOR_BRIGHT_RED && colorMap[i][j] != COLOR_BRIGHT_BLUE && colorMap[i][j] != COLOR_BRIGHT_YELLOW && colorMap[i][j] != COLOR_BRIGHT_GREEN) {
				if (visitMap[playerX + i - 25][playerY + j - 66]) colorMap[i][j] = COLOR_BRIGHT_BLACK;
				else colorMap[i][j] = COLOR_BLACK;
			}
			else colorMap[i][j] = COLOR_BRIGHT_WHITE;
		}
	}
}

void plotLine(int x1, int y1, int x2, int y2, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], int playerX, int playerY, bool visitMap[][MAX_Y + MIN_Y]) {
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
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return;
				//if (x - playerX < MAP_X_START - 25 || x - playerX > MAP_X_END + 25 || y - playerY < MAP_Y_START - 66 || y - playerY > MAP_Y_END + 66) return;
				visitMap[x][y] = true;
				colorMap[x - playerX + 25][y - playerY + 66] = COLOR_CHECKED; //COLOR_BRIGHT_YELLOW;
				if (!isTransparent(map[x][y])) return;
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
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return;
				//if (x - playerX < MAP_X_START - 25 || x - playerX > MAP_X_END + 25 || y - playerY < MAP_Y_START - 66 || y - playerY > MAP_Y_END + 66) return;
				visitMap[x][y] = true;
				colorMap[x - playerX + 25][y - playerY + 66] = COLOR_CHECKED; //COLOR_BRIGHT_GREEN;
				if (!isTransparent(map[x][y])) return;
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
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return;
				//if (x - playerX < MAP_X_START - 25 || x - playerX > MAP_X_END + 25 || y - playerY < MAP_Y_START - 66 || y - playerY > MAP_Y_END + 66) return;
				visitMap[x][y] = true;
				colorMap[x - playerX + 25][y - playerY + 66] = COLOR_CHECKED; //COLOR_BRIGHT_RED;
				if (!isTransparent(map[x][y])) return;
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
				if (x < 0 || y < 0 || x > MAX_X + MIN_X || y > MAX_Y + MIN_Y) return;
				//if (x - playerX < MAP_X_START - 25 || x - playerX > MAP_X_END + 25 || y - playerY < MAP_Y_START - 66 || y - playerY > MAP_Y_END + 66) return;
				visitMap[x][y] = true;
				colorMap[x - playerX + 25][y - playerY + 66] = COLOR_CHECKED; //COLOR_BRIGHT_BLUE;
				
				if(!isTransparent(map[x][y])) return;
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

void drawCircle(int R, int playerX, int playerY, WORD colorMap[][189], int map[][MAX_Y+MIN_Y], bool visitMap[][MAX_Y+MIN_Y]) {
	int x = 0, y = R;
	
	// Initial Process Here
	plotLine(playerX, playerY, playerX + x, playerY + y, colorMap, map, playerX, playerY, visitMap);
	plotLine(playerX, playerY, playerX + -x, playerY + -y, colorMap, map, playerX, playerY, visitMap);
	plotLine(playerX, playerY, playerX + y, playerY + x, colorMap, map, playerX, playerY, visitMap);
	plotLine(playerX, playerY, playerX + -y, playerY + -x, colorMap, map, playerX, playerY, visitMap);

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
}


int sqaure(int a) {
	return a * a;
}