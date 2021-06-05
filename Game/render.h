#include <stdio.h> 
#include "map.h"
#include "MobList.h"
#include "MobAI.h"
#include "Player.h"
#include "resource.h"
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "msimg32.lib")

void init_();
void gotoxy(int x, int y);
void defaultLayout(wchar_t buffer[][189]);
void startLayout(wchar_t defaultBuffer[][189]);
void gameoverLayout(wchar_t defaultBuffer[][189]);
char getPrintChar(int i);
void updateMap(int playerX, int playerY, int renderRangeX, int renderRangeY, int map[][MIN_Y + MAX_Y], int startPosX, int endPosX, int startPosY, int endPosY, wchar_t defaultBuffer[49][189]);
void render();
void ClearScreen();
void generatePopup(int Type, int height, int width, wchar_t buffer[][189], WORD colorMap[][189]);
void updatePlayerInfo(int Health, int maxHealth, int *debuff, wchar_t defaultBuffer[][189], WORD colorMap[][189], int gold, int floor, int level, int exp);
int renderGMap(int BitMapMapping[][MIN_Y + MAX_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom);
void GetBMP(HDC hdc, HDC memdc, HBITMAP image);
int GetDPI(HWND hWnd);
void paintMap(HDC backDC, HDC memdc, HDC hdc, int bitMapMapping[][MAX_Y + MIN_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom);
void Render2DMap(int bitMapMapping[][MAX_Y + MIN_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom);
int returnColor(int t);