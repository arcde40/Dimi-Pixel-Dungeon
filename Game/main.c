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
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "GlobalVariable.h"

bool updateSignal = true;
bool muted = false;

// DIMI PIXEL DUNGEON
// Made By 조영민

// 프로그램 진입점
int main() {
	//key input의 상태를 저장하는 배열
	bool keyInputState[0xA6] = { 0, };
	
	// 방 생성
	srand(time(NULL));
	int map[MIN_X+MAX_X][MIN_Y+MAX_Y] = { {0,} };
	int BitMapMapping[MIN_X + MAX_X][MIN_Y + MAX_Y] = { {0 ,} };
	int moveXY[3] = {-1, 0, 1};
	RoomArrayList* array = initRoomArray();
	// 주어진 범위 내에서 방을 생성한다.
	for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
		putRoom(array, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
	}
	draw(map, array);
	BitMapMapper(map, BitMapMapping);
	MobInfo* temp;

	Player p = { 0, };

	init_(); // 기본 CMD 세팅

	// 기본 GameComponent 생성
	wchar_t defaultBuffer[49][189] = { 0, }; // 출력 버퍼
	WORD colorMap[49][189] = { 0, }; // 출력 컬러 버퍼
	bool visitMap[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, }; // 맵 방문 여부를 저장하는 배열
	MobList* mobList = initMobList(); // 몬스터 저장 ArrayList
	defaultLayout(defaultBuffer); // 출력 버퍼에 기본 레이아웃을 덮어씁니다.
	for (int i = 0; i < 49; i++) { // 사전 출력
		fprintf(stdout, "%ls\n",defaultBuffer[i]);
	}
	
	// Map display (4,6) ~ (45, 129) : Buffering Enabled
	// 마우스 입력 받기 설정
	DWORD mode;
	WORD key;
	COORD pos;
	// 입출력 핸들 받아오기
	HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE COUT = GetStdHandle(STD_OUTPUT_HANDLE);

	bool isMoving = false; // 이동 여부를 저장하는 변수
	PointArrayList* path = NULL; // 이동할 길을 저장하는 ArrayList
	int pathSequence = 0; // 현재 PointArrayList에서 몇번째인지 저장하는 변수

	/*
	* 현재 레이아웃을 저장합니다.
	* 0 = Default Layout
	* 1 = Popup ( Map click disabled )
	*/
	int currentLayout = 0;

	// 마우스 활성화
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);

	wchar_t coloredBuffer[189 * 5] = { 0, };

	for (int i = 0; i < 50; i++) p.debuff[i] = 0;

	// 행동포인트
	float actionPoint = 1;

	// 화면 좌하단의 로그를 저장하는 ArrayList
	LogArrayList* arr = initLogArray();
	int inventoryCurPos = 0; // 현재 인벤토리 커서 위치
	int buttonCurPos = 0; // 현재 인벤토리 버튼 위치
	float walkSpeed = 1; // 걷기 행동력 소모값
	Item* itemMap[MAX_X + MIN_X][MIN_Y + MAX_Y] = { 0, }; // 아이템의 위치를 저장하는 배열

	// Initialize Game Components
	GameComponent component = { 0, }; // 게임에서 쓰이는 GameComponent들을 모아두는 구조체
	GameRandomnizer rnd = { 0, }; // 랜덤값을 저장하는 구조체 (쓰이지 않음)
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

	// 현재 게임의 상태 ( 시작, 게임 중, 종료 )
	// 1 - Opening 2 - Gameing 3 - Ending
	int gameState = 0;
	
	// 게임 난수화
	randomnizePotion(&rnd);
	spawnItem(&component);
	spawnChest(&component);

	int movingFrame = 0; // 무빙 프레임

	// 게임 루프
	while (1) {
		movingFrame++;
		// 쉰다.
		Sleep(MAX_FRAME);

		// 시작점
		if (gameState == 0) {
			//PlaySound(TEXT("theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			int cursor = 0;
			char arrow[3] = "▶";
			int isSaved = _access("save.dpd", 00);
			while (1) {
				// 화면 출력
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

				// 키 입력 확인
				if (CheckKeyPressed(VK_DOWN)) { // 커서 이동
					keyInputState[VK_DOWN] = true;
					if (cursor < 2) cursor++;
					if (cursor == 1 && isSaved) cursor++;
				}
				if (CheckKeyPressed(VK_UP)) { // 커서 이동
					keyInputState[VK_UP] = true;
					if (cursor > 0) cursor--;
					if (cursor == 1 && isSaved) cursor--;
				}
				if (CheckKeyPressed(0x0D)) { // 확인 (엔터)
					keyInputState[0x0D] = true;
					if (cursor == 0) { // New Game이라면
						// 새 게임 세팅
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
						component.gameState = &gameState;

						p.playerX = getStartPos().X;
						p.playerY = getStartPos().Y;
						memset(p.debuff, 0, sizeof(p.debuff));
						p.maxHealth = 100;
						p.Health = 100;
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
						summonMob(mobList, (rand() % MOB_MAX_COUNT - MOB_MIN_COUNT) + MOB_MIN_COUNT, 1, map);
						spawnItem(&component);
						spawnChest(&component);
						p.playerX = getStartPos().X;
						p.playerY = getStartPos().Y;
						p.accuracyConstant = 5;
						p.dodgeConstant = 5;
						for (int i = 0; i < 11; i++) p.debuff[i] = -1;
						char t[100] = { 0, };
						sprintf(t, "작전 구역의 %d층으로 진입했습니다.", ++p.currentFloor);
						putMBS(arr, t);
						gameState = 1;
						PlaySound(NULL, 0, 0);
						//PlaySound(TEXT("game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
						break;
					}
					else if (cursor == 1) { // Continue 라면
						// 불러오기
						gameState = 1;
						component.gameState = &gameState;
						component.player = &p;
						loadSaveFile(&component);
						PlaySound(NULL, 0, 0);
						PlaySound(TEXT("game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
						break;
					}
					else { // Terminate 라면
						// 종료
						exit(0);
					}
				}
				for (int i = 1; i < 0xA6; i++) { // 만약 눌려있지 않다면 해당 keyInputState를 false로 바꿔줍니다.
					if (!GetAsyncKeyState(i)) keyInputState[i] = false;
				}

				Sleep(MAX_FRAME);
			}
		}

		if (gameState == 2) { // 죽었다면!
			// 세이브 파일 삭제
			remove("save.dpd");
			// 게임 종료 화면 띄우기
			gameState = 3;
			
		}

		if (gameState == 3) { // 게임이 종료되었다면
			// 사망 레이아웃 출력
			gotoxy(0, 0);
			gameoverLayout(defaultBuffer);
			defaultLighting(colorMap);
			for (int i = 0; i < 49; i++) {
				applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
				fwprintf(stdout, L"%ls", coloredBuffer);
				if (i != 48) fprintf(stdout, "\n");
			}
			Sleep(1000);
			while (!_kbhit()); // 임의의 키를 누를때까지 대기
			Sleep(500);
			// 처음으로 돌아갑니다.
			gameState = 0;
			continue;
		}
		// 본격 게임 시작!
		// 화면 출력 코드
		if (updateSignal) {
			// stdout 스트림 청소
			fflush(stdout);

			// 기본 레이아웃 적용
			defaultLayout(defaultBuffer);
			gotoxy(0, 0); // 커서 이동
			updateMap(p.playerX, p.playerY, RENDERRANGE_X, RENDERRANGE_Y, map, 4, 45, 6, 129, defaultBuffer); // 지정 좌표에 맵을 그려줍니다.
			defaultLighting(component.colorMap); // 기본 라이팅 적용 (하얀색)
			updatePlayerInfo(p.Health, p.maxHealth, p.debuff, defaultBuffer, colorMap, p.money, p.currentFloor, p.level, p.exp); // 플레이어 정보 (HUD)까지 업데이트

			printLog(defaultBuffer, arr); // 로그 출력
			mapLighting1(p.playerX, p.playerY, 50, colorMap, map, visitMap, &component); // 맵에 라이팅을 적용해줍니다.

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
				if (p.currentFloor == 4) summonMob(mobList, 1, 10, map);
				else if (p.currentFloor == 9) summonMob(mobList, 1, 11, map);
				else summonMob(mobList, rand() % 10 + 6, p.currentFloor, map);
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

			// 인벤토리
			if (currentLayout == 1) generateInventory(&p, defaultBuffer, colorMap, inventoryCurPos, buttonCurPos); // 현재 레이아웃이 인벤토리라면 인벤토리 팝업을 만듭니다.
			

			// 출력
			for (int i = 0; i < 49; i++) { 
				applyColor(colorMap[i], defaultBuffer[i], coloredBuffer); // colorMap을 defaultBuffer에 적용
				fwprintf(stdout, L"%ls", coloredBuffer); // 출력
				if (i != 48) fprintf(stdout, "\n"); // 마지막 줄이라면 \n 생략
			}
			updateSignal = true; // 의미 없습니다.
			
		}
		// map Range: (4,6) ~ (45, 129)

		if (be_input(CIN)) // 마우스 입력
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



						if (isMoving) { // 이미 움직이고 있다면 움직임 취소
							path = NULL;
							pathSequence = 0;
							isMoving = false;
						}

						
						if ((temp = getMobInfoByPosition(p.playerX + x - 25, p.playerY + y - 66, mobList)) != NULL && attackAble(&p, temp, mobList, arr)) { 
							// 만약 클릭한 좌표에 몬스터가 있고 공격 범위 내에 있어 공격할 수 있다면 (attackAble 함수에서 공격 처리)
							// 피격 표시
							colorMap[temp->posX][temp->posY] = COLOR_BRIGHT_RED;
							for (int i = 0; i < 49; i++) { // 출력
								applyColor(colorMap[i], defaultBuffer[i], coloredBuffer);
								fwprintf(stdout, L"%ls", coloredBuffer);
								if (i != 48) fprintf(stdout, "\n");
							}
							Sleep(100); // 잠깐 쉼
							nextTurn(&component, &actionPoint, &(p.equippedWeapon->attackSpeed)); // 다음 턴으로 넘어갑니다.
						}
						else {
							// 만약 클릭한 좌표에 몬스터가 없거나 공격 범위 밖이라 공격할 수 없다면
							path = findPath(map, p.playerX, p.playerY, p.playerX + x - 25, p.playerY + y - 66, mobList, -1); // 그쪽으로 가고 싶은 것으로 간주, 길을 찾습니다.

							if (path != NULL) { // 길이 없는 것이 아니라면
								pathSequence = path->size - 2; // 시작점을 제외하기 위해 -2 해줍니다.
								isMoving = true; // 움직이고 있다는 것을 알려줍니다.
							}
						}


					}
				}
				else if(currentLayout == 1){ // 만약 클릭한 곳이 인벤토리라면
					if ((x >= 8 && x <= 48) && (y <= 130 && y >= 60)) { // 그리고 아이템 목록 구역에 있다면
						if (y < 96) {
							if (inventoryCurPos != (x - 8) / 2) {
								// 적당히 수치를 대입하여 클릭한 곳의 아이템을 선택 처리 합니다.
								inventoryCurPos = (x - 8) / 2;
								buttonCurPos = 0;
								
							}
						}
						else { // 그게 아니라
							if (x >= 37 && x <= 41) {
								if (y >= 96 && y <= 106) { // 첫번째 버튼을 클릭했다면
									if (buttonCurPos == 1) { // 근데 이미 선택되어 있다면
										useItem(&component, getItem(p.inventory, inventoryCurPos)); // 선택된 아이템 사용/착용
									}
									else buttonCurPos = 1; // 아닐경우 그냥 첫번째 버튼으로 옮깁시다.
									
								}
								if (y >= 108 && y <= 118) buttonCurPos = 2; // 두번째 버튼을 클릭?
								if (y >= 120 && y <= 130) buttonCurPos = 3; // 세번째 버튼을 클릭?
							}
						}
					}

				}
			}
		}
		// 만약 게임 화면이라면 (인벤토리가 아니라)
		if (currentLayout == 0) {
			if (isMoving) {
				if (movingFrame >= 10) { // 무빙 프레임이 10 이상이라면
					// 이동 처리
					int prevPlayerX = p.playerX, prevPlayerY = p.playerY;
					p.prevPlayerX = prevPlayerX; p.prevPlayerY = prevPlayerY; // GameComponent의 그것과 연동해줍니다.
					p.playerX = getPoint(path, pathSequence)->x;
					p.playerY = getPoint(path, pathSequence--)->y;
					p.playerX = p.playerX;
					p.playerY = p.playerY;
					if (pathSequence < 0) isMoving = false;
					walkSpeed = 1;
					if (p.equippedArmor->level - p.level > 0) walkSpeed *= (p.equippedArmor->level - p.level) + 1;
					nextTurn(&component, &actionPoint, &walkSpeed);
					movingFrame = 0;
				}
			}
			else { // 지금 움직이고 있지 않다면
				for (int i = 0x0; i < 0x9; i++) { // 텐키 입력 받기
					if (i == 0x4) continue;
					if (GetAsyncKeyState(0x61 + i)) {
						int prevPlayerX = p.playerX, prevPlayerY = p.playerY;
						p.prevPlayerX = prevPlayerX; p.prevPlayerY = prevPlayerY;
						if (isPassable(map[p.playerX - moveXY[i / 3]][p.playerY + moveXY[i % 3]])) {
							p.playerX -= moveXY[i / 3];
							p.playerY += moveXY[i % 3];
							Sleep(50);
							walkSpeed = 1;
							if (p.equippedArmor->level - p.level > 0) walkSpeed *= (p.equippedArmor->level - p.level) + 1;
							nextTurn(&component, &actionPoint, &(walkSpeed));
						}
						break;
					}
				}
			}
			if (GetAsyncKeyState(0x65) && !keyInputState[0x65]) { // 또는 5번을 클릭했다면
				keyInputState[0x65] = true;
				bool t = false;
				for (int k = -p.equippedWeapon->attackRange; k <= p.equippedWeapon->attackRange; k++) { // 공격하거나
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
							float attackSpeed = p.equippedWeapon->attackSpeed;
							if (p.equippedWeapon->level - p.level > 0) attackSpeed *= (p.equippedWeapon->level - p.level) + 1;
							nextTurn(&component, &actionPoint, &(attackSpeed));
							t = true;
							break;
						}
					}
					if (t) break;
				}

				if (!t) { // 공격하지 않았다면 상자 열기
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
			if (CheckKeyPressed(0x60)) { // 0번 클릭시
				keyInputState[0x60] = true;
				nextTurn(&component, &actionPoint, &(p.equippedWeapon->attackSpeed)); // 한턴 쉬고 넘어가기
			}
			// SaveFile test
			// S
			if (CheckKeyPressed(0x53)) { // 세이브 파일 저장
				keyInputState[0x53] = true;
				writeSaveFile(&component);
				putMBS(arr, "세이브 파일이 저장되었습니다.");
			}
			// L
			if (CheckKeyPressed(0x4C)) { // 세이브 파일 불러오기
				keyInputState[0x4C] = true;
				loadSaveFile(&component);
				putMBS(arr, "세이브 파일이 로드되었습니다.");
				continue;
			}
			// M
			if (CheckKeyPressed(0x4D)) {
				keyInputState[0x4D] = true;

				if (!muted){
					PlaySound(NULL, 0, 0);
					putMBS(arr, "배경음악이 음소거 되었습니다.");
					muted = true;
				}
				else {
					PlaySound(TEXT("game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					putMBS(arr, "배경음악을 재생합니다.");
					muted = false;
				}
			}

			// I
			if (GetAsyncKeyState(0x49) && !keyInputState[0x49]) { // 인벤토리 열기
				currentLayout = 1;
				keyInputState[0x49] = true;
			}
		}
		else if (currentLayout == 1) { // 인벤토리에서 키 입력 처리
			if (GetAsyncKeyState(0x1B)) { // ESC버튼 누를 경우 인벤토리 닫기
				currentLayout = 0;
			}
			if (GetAsyncKeyState(0x49) && !keyInputState[0x49]) { // I 버튼 다시 누를 경우 인벤토리 닫기
				currentLayout = 0;
				keyInputState[0x49] = true;
			}
			// Down
			if (GetAsyncKeyState(0x28) && !keyInputState[0x28] && buttonCurPos == 0) { // 커서 옮기기
				if(inventoryCurPos < p.inventory->size-1) inventoryCurPos++;
				keyInputState[0x28] = true;
			}
			// Up
			if (GetAsyncKeyState(0x26) && !keyInputState[0x26] && buttonCurPos == 0) { // 커서 옮기기
				if(inventoryCurPos > 0) inventoryCurPos--;
				keyInputState[0x26] = true;
			}

			// Left
			if (GetAsyncKeyState(0x25) && !keyInputState[0x25]) { // 버튼 옮기기
				if(buttonCurPos > 0) buttonCurPos--;
				keyInputState[0x25] = true;
			}

			// Right
			if (GetAsyncKeyState(0x27) && !keyInputState[0x27]) { // 버튼 옮기기
				if(buttonCurPos < 3) buttonCurPos++;
				keyInputState[0x27] = true;
			}

			if (GetAsyncKeyState(0x0D) && !keyInputState[0x0D]) { // 엔터를 누를시에
				if (buttonCurPos == 1) { // 사용/장착 버튼이라면
					if(p.inventory->size > 0) useItem(&component, getItem(p.inventory, inventoryCurPos)); // 선택된 아이템 사용
				}
			}
		}

		for (int i = 1; i < 0xA6; i++) { // 모든 키 State 바꾸기
			if (!GetAsyncKeyState(i)) keyInputState[i] = false;
		}

		if (p.Health <= 0) gameState = 2; // 만약 플레이어 체력이 다 떨어졌다면 사망 처리
	}
	return 0; // 프로그램 종료

	// 난잡한 코드 읽어주셔서 감사합니다.
	// 상당수의 코드가 중복됩니다. 주석을 자세하게 달지 않은 부분이 중복된 부분이며, 첫번째 그 코드가 나온 곳에 상세하게 설명되어 있습니다.
	// 가끔씩 영어로 작성된 주석은 실제 제작하면서 달은 주석으로, 가장 핵심적인 내용을 담고 있습니다. 한글보다는 영어가 편한지라..
}