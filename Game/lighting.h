﻿#pragma once
#include <Windows.h>
#include <stdbool.h>

#include "map.h"
#include "GlobalVariable.h"
#include "GameManager.h"

#define	ABS(x)			( ((x)<0)?-(x):(x) )

void defaultLighting(WORD colorMap[][189]);
void mapLighting(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], bool visitMap[][MAX_Y + MIN_Y]);
void mapLighting1(int playerX, int playerY, int renderRange, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], bool visitMap[][MAX_Y + MIN_Y], GameComponent* g);
void applyColor(WORD* colorMap, wchar_t* buffer, wchar_t* appliedBuffer);
void plotLine(int x1, int y1, int x2, int y2, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], int playerX, int playerY, bool visitMap[][MAX_Y + MIN_Y]);
void drawCircle(int R, int playerX, int playerY, WORD colorMap[][189], int map[][MAX_Y + MIN_Y], bool visitMap[][MAX_Y + MIN_Y]);
int sqaure(int a);