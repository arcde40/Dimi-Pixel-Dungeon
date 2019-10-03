#include <Windows.h>
#include <stdbool.h>

#include "GlobalVariable.h"

void defaultLighting(WORD colorMap[][189]) {
	for (int i = 0; i < 49; i++) {
		for (int j = 0; j < 189; j++) {
			colorMap[i][j] = 7;
		}
	}
}

void mapLighting(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y+MIN_Y], bool visitMap[][MAX_Y+MIN_Y]) {
	for(int i = )
}