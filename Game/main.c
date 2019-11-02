#include <stdio.h>
#include "room.h"
#include "roomarraylist.h"
#include "map.h"
#include "render.h"
#include "struct.h"
#include "pathfinding.h"
#include "InputHandler.h"
#include "lighting.h"
#include "MobAI.h"
#include "MobList.h"
#include "Player.h"
#include "Logger.h"
#include "LogArrayList.h"
#include "Item.h"
#include "GameManager.h"
#include "PlayerGUI.h"
#include <time.h>
#include <locale.h>
#include <Windows.h>
#include <string.h>

#include "GlobalVariable.h"


int main() {
	//setlocale(LC_ALL, "");
	//setlocale(LC_ALL, "");
	//_wsetlocale(LC_ALL, L"Korean");
	bool keyInputState[0xA6] = { 0, };

	srand(time(NULL));
	int map[MIN_X+MAX_X][MIN_Y+MAX_Y] = { {0,} };
	int moveXY[3] = {-1, 0, 1};
	RoomArrayList* array = initRoomArray();
	for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
		putRoom(array, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
	}
	draw(map, array);

	MobInfo* temp;


	int playerX = getStartPos().X, playerY = getStartPos().Y;
	init_();
	wchar_t defaultBuffer[49][189] = { 0, };
	WORD colorMap[49][189] = { 0, };
	bool visitMap[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, };
	MobList* mobList = initMobList();
	defaultLayout(defaultBuffer);
	for (int i = 0; i < 49; i++) {
		fprintf(stdout, "%ls\n",defaultBuffer[i]);
	}
	/*while (1) {
		Sleep(100);
		char ch = getch();
		for (int i = 0; i < 49; i++) {
			fprintf(stdout, "%s\n", defaultBuffer[i]);
		}
	}*/
	// Map display (4,6) ~ (45, 129) : Buffering Enabled
	DWORD mode;
	WORD key;
	COORD pos;
	HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE COUT = GetStdHandle(STD_OUTPUT_HANDLE);

	bool isMoving = false;
	PointArrayList* path = NULL;
	int pathSequence = 0;

	/*
	* 0 = Default Layout
	* 1 = Popup ( Map click disabled )
	*/
	int currentLayout = 0;

	// 마우스 활성화
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	// Debug Property Start
	wchar_t coloredBuffer[189 * 5] = { 0, };
	MobInfo mobInfo = { 100, "테스트몹", 100, 100, 50, 75, playerX - 1, playerY - 1, MOB_BEHAVE_HOSTILE, false, 5, false, NULL, 0};
	// Important!
	Player p = { 0, };
	p.maxHealth = 999;
	p.Health = 999;
	Item item = { "나무 검", "맞아도 하나도 아프지 않다.", 0, ITEM_WEAPON, 10, 20, 1, 1 };
	Item item2 = { "나무 방패", "&6나무&f로 만들어져 있다.|상당히 약해서 이것으로 무엇을 막을 수 있을지 가늠이 가지 않는다.|혹시 이걸로 막았다가 부서지면 어떡해..", 0, ITEM_ARMOR, 10, 20 , 1, 1 };
	Item item3 = { "테스트 아이템", "줄바꿈 테스트!|이예!|색코드 테스트!|&8오예!|&9오예!!|&a오예!!|&b오예!!|&c오예!!|&d오예!!|&e오예!!|&f오예!!", 0, ITEM_UTILITY, 10, 20 , 1, 1 };

	p.inventory[0] = &item;
	p.inventory[1] = &item2;
	p.inventory[2] = &item3;

	p.equippedWeapon = &item;
	putMobInfo(mobList, &mobInfo);
	// Debug Property End

	LogArrayList* arr = initLogArray();
	summonMob(mobList, 15, 1, map);
	putMBS(arr, "작전 구역의 1층으로 진입했습니다.");
	int inventoryCurPos = 0;
	while (1) {
		p.playerX = playerX;
		p.playerY = playerY;

		Sleep(MAX_FRAME);
		//ClearScreen();
		//system("cls");
		fflush(stdout);

		defaultLayout(defaultBuffer);
		gotoxy(0, 0);
		updateMap(playerX, playerY, RENDERRANGE_X, RENDERRANGE_Y, map, 4, 45, 6, 129, defaultBuffer);
		defaultLighting(colorMap);
		updatePlayerInfo(p.Health, p.maxHealth, defaultBuffer);
		
		
		printLog(defaultBuffer, arr);
		mapLighting1(playerX, playerY, 50, colorMap, map, visitMap);


		// Display Mob
		for (int i = 0; i < mobList->size; i++) {
			MobInfo* info = getMobInfo(mobList, i);
			// 25, 66
			if (ABS(playerX - info->posX) < RENDERRANGE_X && ABS(playerY - info->posY) < RENDERRANGE_Y) {
				if (colorMap[-playerX + info->posX + 25][-playerY + info->posY + 66] == COLOR_BRIGHT_WHITE) defaultBuffer[-playerX + info->posX + 25][-playerY + info->posY + 66] = 'E';
			}
		}

		if(currentLayout == 1) generateInventory(&p, defaultBuffer, colorMap, inventoryCurPos);
		
		//Sleep(100);
		for (int i = 0; i < 49; i++) {
			applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
			fwprintf(stdout, L"%ls", coloredBuffer);
			if (i != 48) fprintf(stdout, "\n");
		}
		
		// map Range: (4,6) ~ (45, 129)

		if (be_input(CIN))
		{
			if (get_input(&key, &pos, COUT, CIN) != 0)
			{
				MOUSE_EVENT;
				int y = pos.X;
				int x = pos.Y;
				if (currentLayout == 0) {
					if ((x > 4 && x < 45) && (y > 6 && y < 129)) {
						// Let's calculate ACTUAL coordinate!
						// X: Actual player coordinate : 4 + renderRangeX + 1 = 25 (Because X range is 4 ~ 45 (dist: 41) which is odd; so plus 1)
						// Y: Actual player coordinate : 6 + renderRangeY = 66
						// Clicked Coordinate (CC)
						// CC - 25 = dx CC - 66 = dy
						// playerX + dx = actual coordinate 



						if (isMoving) {
							path = NULL;
							pathSequence = 0;
							isMoving = false;
						}

						if ((temp = getMobInfoByPosition(playerX + x - 25, playerY + y - 66, mobList)) != NULL && attackAble(&p, temp, mobList, arr)) {
							for (int i = 0; i < mobList->size; i++) {
								enemyBehave(getMobInfo(mobList, i), mobList, playerX, playerY, map, &p, playerX, playerY, arr);
							}
						}
						else {
							path = findPath(map, playerX, playerY, playerX + x - 25, playerY + y - 66, mobList);

							if (path != NULL) {
								pathSequence = path->size - 2;
								isMoving = true;
							}
						}


					}
				}
			}
		}
		if (currentLayout == 0) {
			if (isMoving) {
				int prevPlayerX = playerX, prevPlayerY = playerY;
				playerX = getPoint(path, pathSequence)->x;
				playerY = getPoint(path, pathSequence--)->y;
				for (int i = 0; i < mobList->size; i++) {
					enemyBehave(getMobInfo(mobList, i), mobList, playerX, playerY, map, &p, prevPlayerX, prevPlayerY, arr);
				}

				if (pathSequence < 0) isMoving = false;
			}
			else {
				for (int i = 0x0; i < 0x9; i++) {
					if (GetAsyncKeyState(0x61 + i)) {
						playerY += moveXY[i % 3];
						playerX -= moveXY[i / 3];
						Sleep(100);
						break;
					}
				}
			}
			// I
			if (GetAsyncKeyState(0x49) && !keyInputState[0x49]) {
				currentLayout = 1;
				keyInputState[0x49] = true;
			}
		}
		else if (currentLayout == 1) {
			if (GetAsyncKeyState(0x1B)) {
				currentLayout = 0;
			}
			if (GetAsyncKeyState(0x49) && !keyInputState[0x49]) {
				currentLayout = 0;
				keyInputState[0x49] = true;
			}
			// Down
			if (GetAsyncKeyState(0x28) && !keyInputState[0x28]) {
				inventoryCurPos++;
				keyInputState[0x28] = true;
			}
			// Up
			if (GetAsyncKeyState(0x26) && !keyInputState[0x26]) {
				inventoryCurPos--;
				keyInputState[0x26] = true;
			}
		}

		for (int i = 1; i < 0xA6; i++) {
			if (!GetAsyncKeyState(i)) keyInputState[i] = false;
		}

		if (p.Health <= 0) break;


		
		/*char ch = getch();
		switch (ch)
		{
		case 'w':
			if (isPassable(map[playerX - 1][playerY]))
				if (playerX > 0) playerX--;
			break;
		case 's':
			if (isPassable(map[playerX + 1][playerY]))
				if (playerX < 70) playerX++;
			break;
		case 'a':
			if (isPassable(map[playerX][playerY - 1]))
				if (playerY > 0) playerY--;
			break;
		case 'd':
			if (isPassable(map[playerX][playerY + 1]))
				if (playerY < 95) playerY++;
		}*/
	}
	return 0;
}