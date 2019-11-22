#define _CRT_SECURE_NO_WARNINGS

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
#include <io.h>
#include <string.h>

#include "GlobalVariable.h"

bool updateSignal = true;


// DIMI PIXEL DUNGEON
// Made By 조영민

// 프로그램 진입점
int main() {
	bool keyInputState[0xA6] = { 0, };

	srand(time(NULL));
	int map[MIN_X+MAX_X][MIN_Y+MAX_Y] = { {0,} };
	int BitMapMapping[MIN_X + MAX_X][MIN_Y + MAX_Y] = { {0 ,} };
	int moveXY[3] = {-1, 0, 1};
	RoomArrayList* array = initRoomArray();
	for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
		putRoom(array, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
	}
	draw(map, array);
	BitMapMapper(map, BitMapMapping);
	MobInfo* temp;



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
	//MobInfo mobInfo = { 0, 100, "테스트몹", 100, 100, 50, 75, playerX - 1, playerY - 1, MOB_BEHAVE_HOSTILE, false, 5, false, NULL, 0};
	// Important!
	Player p = { 0, };
	p.playerX = getStartPos().X;
	p.playerY = getStartPos().Y;
	p.maxHealth = 999;
	p.Health = 999;
	p.currentFloor = 1;
	p.inventory = initItemArray();
	p.keyList = initArrayList();
	for (int i = 0; i < 50; i++) p.debuff[i] = 0;
	Item weapon = { "엑스칼리버", "전설의 검 엑스칼리버. 뽑은 자만이 사용할 수 있다는 검이다. 이것을 왜 내가 가지고 있는지는 기억이 나지 않는다.", 0, ITEM_WEAPON, 100, 1230, 1853, -1, 1, 1, 1, 1};
	//Item item = { "나무 검", "맞아도 하나도 아프지 않다.", 0, ITEM_WEAPON, 1, 10, 20, -1, 1, 1 ,0.5 ,1};
	//Item item2 = { "나무 방패", "&6나무&f로 만들어져 있다.|상당히 약해서 이것으로 무엇을 막을 수 있을지 가늠이 가지 않는다.|혹시 이걸로 막았다가 부서지면 어떡해..", 0, ITEM_ARMOR, 1, 10, 20 , 10, 1, 1 };
	//Item item3 = { "테스트 아이템", "줄바꿈 테스트!|이예!|색코드 테스트!|&8오예!|&9오예!!|&a오예!!|&b오예!!|&c오예!!|&d오예!!|&e오예!!|&f오예!!", 0, ITEM_UTILITY, 1, 10, 20 , -1, 1, 1 };

	//p.inventory[0] = &item;
	//p.inventory[1] = &item2;
	//p.inventory[2] = &item3;
	
	float actionPoint = 1;
	p.equippedWeapon = &weapon;
	//putMobInfo(mobList, &mobInfo);
	// Debug Property End

	LogArrayList* arr = initLogArray();
	//summonMob(mobList, 15, 1, map);
	//putMBS(arr, "작전 구역의 1층으로 진입했습니다.");
	int inventoryCurPos = 0;
	int buttonCurPos = 0;
	float walkSpeed = 1;
	Item* itemMap[MAX_X + MIN_X][MIN_Y + MAX_Y] = { 0, };
	// Initialize Game Components
	GameComponent component = { 0, };
	GameRandomnizer rnd = { 0, };
	component.player = &p;
	component.mobList = mobList;
	component.logList = arr;
	component.roomList = array;
	component.defaultBuffer = defaultBuffer;
	component.cursorPosition = &inventoryCurPos;
	component.buttonCurPosition = &buttonCurPos;
	component.currentLayout = &currentLayout;
	component.colorMap = colorMap;
	component.map = map;
	component.visitmap = visitMap;
	component.itemPos = initPointArray();
	component.itemMap = itemMap;
	INPUT_RECORD irInBuf[128];
	DWORD cNumRead;
	WCHAR title[2048] = { 0 , };
	GetConsoleTitle(title, 2048);
	HWND hWnd = FindWindow(NULL, title);
	SetConsoleTitle(title);
	HDC hDC, hMemDC;
	static HDC hBackDC;
	HBITMAP hBitmap, hOldBitmap, hBackBitmap;
	BITMAP Bitmap;
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);
	hDC = GetDC(hWnd);

	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	hBitmap = LoadBitmap(GetModuleHandle(NULL), IDB_BITMAP5);
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hBitmap);
	//loadSaveFile(&component);
	int movingFrame = 0;

	// 1 - Opening 2 - Gameing 3 - Ending
	int gameState = 0;
	
	randomnizePotion(&rnd);
	spawnItem(&component);
	spawnChest(&component);
	while (1) {
		movingFrame++;
		/*while (1) {
			POINT c;
			 GetCursorPos(&c);
			 ScreenToClient(GetConsoleWindow(), &c);
			 gotoxy(0, 0);
			 printf("%d %d      ", c.x, c.y);
		}*/
		/*for (int i = 0; i < MIN_X + MAX_X; i++) {
			for (int j = 0; j < MAX_Y + MIN_Y; j++) {
				char c;
				switch (BitMapMapping[i][j]) {
				case 0x11: c = 'a'; break;
				case 0x12: c = 'r'; break;
				case 0x13: c = '-'; break;
				case 0x14: c = ')'; break;
				case 0x15: c = '|'; break;
				case 0x16: c = 's'; break;
				case 0x17: c = 'j'; break;
				default: c = ' '; break;
				}
				printf("%c", c);
			}
			printf("\n");
		}*/
		Sleep(MAX_FRAME);
		//ClearScreen();
		//system("cls");

		if (gameState == 0) {
			int cursor = 0;
			char arrow[3] = "▶";
			int isSaved = _access("save.dpd", 00);
			while (1) {
				gotoxy(0, 0);
				defaultLighting(colorMap);
				startLayout(defaultBuffer);
				mbstowcs(&defaultBuffer[29 + 3 * cursor][81], "▶", strlen("▶"));
				if (isSaved) for (int i = 81; i < 91; i++) colorMap[32][i] = COLOR_WHITE;
				for (int i = 0; i < 49; i++) {
					applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
					fwprintf(stdout, L"%ls", coloredBuffer);
					if (i != 48) fprintf(stdout, "\n");
				}

				if (CheckKeyPressed(VK_DOWN)) {
					keyInputState[VK_DOWN] = true;
					if (cursor < 2) cursor++;
					if (cursor == 1 && isSaved) cursor++;
				}
				if (CheckKeyPressed(VK_UP)) {
					keyInputState[VK_UP] = true;
					if (cursor > 0) cursor--;
					if (cursor == 1 && isSaved) cursor--;
				}
				if (CheckKeyPressed(0x0D)) {
					keyInputState[0x0D] = true;
					if (cursor == 0) {
						free(component.itemPos);
						free(component.roomList);
						free(component.mobList);
						free(p.inventory);
						memset(&p, 0, sizeof(p));
						memset(map, 0, sizeof(map));
						memset(visitMap, 0, sizeof(visitMap));
						memset(itemMap, 0, sizeof(itemMap));
						component.itemPos = initPointArray();
						component.roomList = initRoomArray();
						for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
							putRoom(component.roomList, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
						}
						draw(map, component.roomList);

						p.playerX = getStartPos().X;
						p.playerY = getStartPos().Y;
						p.maxHealth = 999;
						p.Health = 999;
						p.currentFloor = 0;
						p.inventory = initItemArray();
						p.keyList = initArrayList();
						Item* weapon = cpyFromItemLib(35);
						Item* armor = cpyFromItemLib(36);
						weapon->amount++; armor->amount++;
						p.equippedWeapon = weapon;
						p.equippedArmor = armor;
						component.player = &p;
						mobList = initMobList();
						component.mobList = mobList;
						summonMob(mobList, 15, 1, map);
						spawnItem(&component);
						spawnChest(&component);
						p.playerX = getStartPos().X;
						p.playerY = getStartPos().Y;
						char t[100] = { 0, };
						sprintf(t, "작전 구역의 %d층으로 진입했습니다.", ++p.currentFloor);
						putMBS(arr, t);
						gameState = 1;
						break;
					}
					else if (cursor == 1) {
						gameState = 1;
						loadSaveFile(&component);
						break;
					}
					else {
						exit(0);
					}
				}
				for (int i = 1; i < 0xA6; i++) {
					if (!GetAsyncKeyState(i)) keyInputState[i] = false;
				}

				Sleep(MAX_FRAME);
			}
		}

		if (gameState == 2) {
			remove("save.dpd");
			gameState = 3;
			
		}

		if (gameState == 3) {
			gotoxy(0, 0);
			gameoverLayout(defaultBuffer);
			defaultLighting(colorMap);
			for (int i = 0; i < 49; i++) {
				applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
				fwprintf(stdout, L"%ls", coloredBuffer);
				if (i != 48) fprintf(stdout, "\n");
			}
			while (!_kbhit());
			Sleep(500);
			gameState = 0;
			continue;
		}
		// 화면 출력 코드
		if (updateSignal) {
			fflush(stdout);

			defaultLayout(defaultBuffer);
			gotoxy(0, 0);
			updateMap(p.playerX, p.playerY, RENDERRANGE_X, RENDERRANGE_Y, map, 4, 45, 6, 129, defaultBuffer);
			defaultLighting(component.colorMap);
			updatePlayerInfo(p.Health, p.maxHealth, p.debuff, defaultBuffer, colorMap);

			printLog(defaultBuffer, arr);
			mapLighting1(p.playerX, p.playerY, 50, colorMap, map, visitMap);

			// Display Mob
			for (int i = 0; i < mobList->size; i++) {
				MobInfo* info = getMobInfo(mobList, i);
				// 25, 66
				if (ABS(p.playerX - info->posX) < RENDERRANGE_X && ABS(p.playerY - info->posY) < RENDERRANGE_Y) {
					if (colorMap[-p.playerX + info->posX + 25][-p.playerY + info->posY + 66] == COLOR_BRIGHT_WHITE) defaultBuffer[-p.playerX + info->posX + 25][-p.playerY + info->posY + 66] = 'E';
				}
			}

			
			// Display Item && Obtain item
			for (int i = 0; i < component.itemPos->size; i++) {
				POINT_P* info = getPoint(component.itemPos, i);
				if (ABS(p.playerX - info->x) < RENDERRANGE_X && ABS(p.playerY - info->y) < RENDERRANGE_Y) {
					if (colorMap[-p.playerX + info->x + 25][-p.playerY + info->y + 66] == COLOR_BRIGHT_WHITE) {
						switch (component.itemMap[info->x][info->y]->id) {
						case 998: defaultBuffer[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = 'K'; colorMap[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = COLOR_BRIGHT_YELLOW; break;
						case 999: defaultBuffer[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = 'K'; colorMap[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = COLOR_BRIGHT_BLUE; break;
						case 1000: defaultBuffer[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = '$'; break;
						default: defaultBuffer[-p.playerX + info->x + 25][-p.playerY + info->y + 66] = '?';
						}

					}
				}
			}
			for (int i = 0; i < component.itemPos->size; i++) {
				POINT_P* info = getPoint(component.itemPos, i);
				if (p.playerX == info->x && p.playerY == info->y) {
					Item* item = component.itemMap[info->x][info->y];
					if (item->id >= 998) {
						switch (item->id) {
						case 998: put(p.keyList, 2); break;
						case 999: put(p.keyList, 3); break;
						case 1000: p.money += item->amount; break;
						}
						component.itemMap[info->x][info->y] = NULL;
						removePoint(component.itemPos, i);
						char t[100] = { 0, };
						sprintf(t, "%s을(를) 획득했다!", item->name);
						putMBS(arr, t);
						free(item);
					}
					else {
						Item* rItem = cpyFromItemLib(item->id);
						component.itemMap[info->x][info->y] = NULL;
						removePoint(component.itemPos, i);
						rItem->enhance = item->enhance;
						rItem->metadata = item->metadata;
						rItem->amount = item->amount;
						char t[100] = { 0, };
						sprintf(t, "%s을(를) 획득했다!", rItem->name);
						putMBS(arr, t);
						putItem(p.inventory, rItem);
						free(item);
					}
				}
			}
			// 내려가는 코드
			if (map[p.playerX][p.playerY] == 22) {
				free(component.itemPos);
				free(component.roomList);
				free(component.mobList);
				memset(map, 0, sizeof(map));
				memset(visitMap, 0, sizeof(visitMap));
				memset(itemMap, 0, sizeof(itemMap));
				component.itemPos = initPointArray();
				component.roomList = initRoomArray();
				for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
					putRoom(component.roomList, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
				}
				
				draw(map, component.roomList);
				mobList = initMobList();
				summonMob(mobList, 15, 1, map);
				component.mobList = mobList;
				spawnItem(&component);
				spawnChest(&component);
				p.playerX = getStartPos().X;
				p.playerY = getStartPos().Y;
				char t[100] = { 0, };
				sprintf(t, "작전 구역의 %d층으로 진입했습니다.", ++p.currentFloor);
				putMBS(arr, t);
				continue;
			}


			if (currentLayout == 1) generateInventory(&p, defaultBuffer, colorMap, inventoryCurPos, buttonCurPos);
			

			for (int i = 0; i < 49; i++) {
				applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
				fwprintf(stdout, L"%ls", coloredBuffer);
				if (i != 48) fprintf(stdout, "\n");
			}
			updateSignal = true;
			//Render2DMap(BitMapMapping, &p, 45, 40, 2);
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

						if ((temp = getMobInfoByPosition(p.playerX + x - 25, p.playerY + y - 66, mobList)) != NULL && attackAble(&p, temp, mobList, arr)) {
							colorMap[temp->posX][temp->posY] = COLOR_BRIGHT_RED;
							for (int i = 0; i < 49; i++) {
								applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
								fwprintf(stdout, L"%ls", coloredBuffer);
								if (i != 48) fprintf(stdout, "\n");
							}
							Sleep(100);
							nextTurn(&component, &actionPoint, &(p.equippedWeapon->attackSpeed));
						}
						else {
							path = findPath(map, p.playerX, p.playerY, p.playerX + x - 25, p.playerY + y - 66, mobList, -1);

							if (path != NULL) {
								pathSequence = path->size - 2;
								isMoving = true;
							}
						}


					}
				}
				else if(currentLayout == 1){
					if ((x >= 8 && x <= 48) && (y <= 130 && y >= 60)) {
						if (y < 96) {
							if (inventoryCurPos != (x - 8) / 2) {
								inventoryCurPos = (x - 8) / 2;
								buttonCurPos = 0;
								
							}
						}
						else {
							if (x >= 37 && x <= 41) {
								if (y >= 96 && y <= 106) {
									if (buttonCurPos == 1) {
										useItem(&component, getItem(p.inventory, inventoryCurPos));
									}
									else buttonCurPos = 1;
									
								}
								if (y >= 108 && y <= 118) buttonCurPos = 2;
								if (y >= 120 && y <= 130) buttonCurPos = 3;
							}
						}
					}

				}
			}
		}
		if (currentLayout == 0) {
			if (isMoving) {
				if (movingFrame >= 10) {
					int prevPlayerX = p.playerX, prevPlayerY = p.playerY;
					p.prevPlayerX = prevPlayerX; p.prevPlayerY = prevPlayerY;
					p.playerX = getPoint(path, pathSequence)->x;
					p.playerY = getPoint(path, pathSequence--)->y;
					p.playerX = p.playerX;
					p.playerY = p.playerY;
					if (pathSequence < 0) isMoving = false;
					nextTurn(&component, &actionPoint, &walkSpeed);
					movingFrame = 0;
				}
			}
			else {
				for (int i = 0x0; i < 0x9; i++) {
					if (i == 0x4) continue;
					if (GetAsyncKeyState(0x61 + i)) {
						int prevPlayerX = p.playerX, prevPlayerY = p.playerY;
						p.prevPlayerX = prevPlayerX; p.prevPlayerY = prevPlayerY;
						if (isPassable(map[p.playerX - moveXY[i / 3]][p.playerY + moveXY[i % 3]])) {
							p.playerX -= moveXY[i / 3];
							p.playerY += moveXY[i % 3];
							Sleep(50);
							nextTurn(&component, &actionPoint, &(walkSpeed));
						}
						break;
					}
				}
			}
			if (GetAsyncKeyState(0x65) && !keyInputState[0x65]) {
				keyInputState[0x65] = true;
				bool t = false;
				for (int k = -p.equippedWeapon->attackRange; k <= p.equippedWeapon->attackRange; k++) {
					for (int l = -p.equippedWeapon->attackRange; l <= p.equippedWeapon->attackRange; l++) {
							if ((temp = getMobInfoByPosition(p.playerX+k, p.playerY+l, mobList)) != NULL && attackAble(&p, temp, mobList, arr)) {
							colorMap[temp->posX - p.playerX + 25][temp->posY - p.playerY + 66] = COLOR_BRIGHT_RED;
							gotoxy(0, 0);
							for (int i = 0; i < 49; i++) {
								applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
								fwprintf(stdout, L"%ls", coloredBuffer);
								if (i != 48) fprintf(stdout, "\n");
							}
							Sleep(100);
							nextTurn(&component, &actionPoint, &(p.equippedWeapon->attackSpeed));
							t = true;
							break;
						}
					}
					if (t) break;
				}

				if (!t) {
					for (int i = -1; i <= 1; i++) {
						for (int j = -1; j <= 1; j++) {
							if (map[p.playerX + i][p.playerY + j] == 30 || map[p.playerX + i][p.playerY + j] == 31) {
								openChest(&component, p.playerX + i, p.playerY + j);
								t = true;
								break;
							}
						}
						if (t) break;
					}
				}
				
			}
			if (CheckKeyPressed(0x60)) {
				keyInputState[0x60] = true;
				nextTurn(&component, &actionPoint, &(p.equippedWeapon->attackSpeed));
			}
			// SaveFile test
			// S
			if (CheckKeyPressed(0x53)) {
				keyInputState[0x53] = true;
				writeSaveFile(&component);
				putMBS(arr, "세이브 파일이 저장되었습니다.");
			}
			// L
			if (CheckKeyPressed(0x4C)) {
				keyInputState[0x4C] = true;
				loadSaveFile(&component);
				putMBS(arr, "세이브 파일이 로드되었습니다.");
				continue;
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
			if (GetAsyncKeyState(0x28) && !keyInputState[0x28] && buttonCurPos == 0) {
				if(inventoryCurPos < p.inventory->size-1) inventoryCurPos++;
				keyInputState[0x28] = true;
			}
			// Up
			if (GetAsyncKeyState(0x26) && !keyInputState[0x26] && buttonCurPos == 0) {
				if(inventoryCurPos > 0) inventoryCurPos--;
				keyInputState[0x26] = true;
			}

			// Left
			if (GetAsyncKeyState(0x25) && !keyInputState[0x25]) {
				if(buttonCurPos > 0) buttonCurPos--;
				keyInputState[0x25] = true;
			}

			// Right
			if (GetAsyncKeyState(0x27) && !keyInputState[0x27]) {
				if(buttonCurPos < 3) buttonCurPos++;
				keyInputState[0x27] = true;
			}

			if (GetAsyncKeyState(0x0D) && !keyInputState[0x0D]) {
				if (buttonCurPos == 1) {
					if(p.inventory->size > 0) useItem(&component, getItem(p.inventory, inventoryCurPos));
				}
			}
		}

		for (int i = 1; i < 0xA6; i++) {
			if (!GetAsyncKeyState(i)) keyInputState[i] = false;
		}

		if (p.Health <= 0) gameState = 2;


		
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