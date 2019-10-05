#pragma once
#include <Windows.h>
#include <stdbool.h>

#include "GlobalVariable.h"

#define	ABS(x)			( ((x)<0)?-(x):(x) )

void defaultLighting(WORD colorMap[][189]);
void mapLighting(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], bool visitMap[][MAX_Y + MIN_Y]);
char* applyColor(WORD* colorMap, char* buffer);