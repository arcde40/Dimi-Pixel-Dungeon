#pragma once
#include <stdio.h> 
#include <Windows.h>
#include "map.h"
#include "MobList.h"
#include "MobAI.h"

void init_();
void gotoxy(int x, int y);
void defaultLayout(wchar_t buffer[][189]);
char getPrintChar(int i);
void updateMap(int playerX, int playerY, int renderRangeX, int renderRangeY, int map[][MIN_Y + MAX_Y], int startPosX, int endPosX, int startPosY, int endPosY, wchar_t defaultBuffer[49][189], MobList* mobList);
void render();
void ClearScreen();
void generatePopup(int Type, int height, int width, wchar_t buffer[][189], WORD colorMap[][189]);
void updatePlayerInfo(int Health, int maxHealth, wchar_t defaultBuffer[][189]);