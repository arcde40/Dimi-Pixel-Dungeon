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

char* applyColor(WORD* colorMap, char* buffer) {
	int prev = -1, scope = 0;
	char appliedBuffer[189 * 5] = { 0, };
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

	return appliedBuffer;
}

int sqaure(int a) {
	return a * a;
}