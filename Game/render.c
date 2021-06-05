#define _CRT_SECURE_NO_WARNINGS
#include "render.h"
// X: 188 Y: 49 최대

// 콘솔 사이즈를 결정합니다.
void init_() {
	system("mode con cols=188 lines=49 | title DIMI PIXEL DUNGEON");
}

// 해당 좌표로 커서를 이동합니다.
void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(consoleHandle, coord);
}

// 기본 레이아웃을 버퍼에 복사합니다.
void defaultLayout(wchar_t buffer[][189]) {
	// Y: 30~46 X: 134~183
	char temp[49][188] = {
		"###########################################################################################################################################################################################",
"#                                                                                                                                                                                         #",
"#   #############################################################################################################################                     DIMI PIXEL DUNGEON                  #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #     HP: [                                  ] 000/000    #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #   ###################     LOG    ###################    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #                                                                                                                           #   #                                                #    #",
"#   #############################################################################################################################   ##################################################    #",
"#                                                                                                                                                                                         #",
"#                                                                                                                                                                                         #",
"###########################################################################################################################################################################################"
	};
	for (int i = 0; i < 49; i++) {
		for (int j = 0; j < 188; j++) buffer[i][j] = temp[i][j];
	}
}

// 시작 레이아웃을 버퍼에 복사합니다.
void startLayout(wchar_t defaultBuffer[][189]) {
	char temp[49][188] = {
	"###########################################################################################################################################################################################",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                       ________  .__        .__         .__              .__    ________                                                                                 #",
		"#                                       \\______ \\ |__| _____ |__| ______ |__|__  ___ ____ |  |   \\______ \\  __ __  ____    ____   ____  ____   ____                                       #",
		"#                                        |    |  \\|  |/     \\|  | \\____ \\|  \\  \\/  // __ \\|  |    |    |  \\|  |  \\/    \\  / ___\\_/ __ \\/  _ \\ /    \\                                      #",
		"#                                        |    `   \\  |  Y Y  \\  | |  |_> >  |>    <\\  ___/|  |__  |    `   \\  |  /   |  \\/ /_/  >  ___(  <_> )   |  \\                                     #",
		"#                                       /_______  /__|__|_|  /__| |   __/|__/__/\\_ \\\\___  >____/ /_______  /____/|___|  /\\___  / \\___  >____/|___|  /                                     #",
		"#                                               \\/         \\/     |__|            \\/    \\/               \\/           \\//_____/      \\/           \\/                                      #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                  New Game                                                                                               #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                  Continue                                                                                               #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                  TERMINATE                                                                                              #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"#                                                                                                                                                                                         #",
		"###########################################################################################################################################################################################"
	};
	for (int i = 0; i < 49; i++) {
		for (int j = 0; j < 188; j++) defaultBuffer[i][j] = temp[i][j];
	}
}

// 게임 오버 레이아웃을 버퍼로 복사합니다.
void gameoverLayout(wchar_t defaultBuffer[][189]) {
	char temp[49][188] = {
		"###########################################################################################################################################################################################",
"#                                                                                                                                                                                         #",
"#                                                                                   uuuuuuu                                                                                               #",
"#                                                                                 uu$$$$$$$$$$$uu                                                                                         #",
"#                                                                              uu$$$$$$$$$$$$$$$$$uu                                                                                      #",
"#                                                                             u$$$$$$$$$$$$$$$$$$$$$u                                                                                     #",
"#                                                                           u$$$$$$$$$$$$$$$$$$$$$$$u                                                                                     #",
"#                                                                          u$$$$$$$$$$$$$$$$$$$$$$$$$u                                                                                    #",
"#                                                                          u$$$$$$$$$$$$$$$$$$$$$$$$$u                                                                                    #",
"#                                                                          u$$$$$$\"   \"$$$\"   \"$$$$$$u                                                                                #",
"#                                                                          \"$$$$\"      u$u       $$$$\"                                                                                 #",
"#                                                                           $$$u       u$u       u$$$                                                                                     #",
"#                                                                           $$$u      u$$$u      u$$$                                                                                     #",
"#                                                                            \"$$$$uu$$$   $$$uu$$$$\"                                                                                    #",
"#                                                                             \"$$$$$$$\"   \"$$$$$$$\"                                                                                   #",
"#                                                                              u$$$$$$$u$$$$$$$u                                                                                          #",
"#                                                                                u$\"$\"$\"$\"$\"$\"$u                                                                                    #",
"#                                                                     uuu        $$u$ $ $ $ $u$$       uuu                                                                                #",
"#                                                                    u$$$$        $$$$$u$u$u$$$       u$$$$                                                                               #",
"#                                                                     $$$$$uu      \"$$$$$$$$$\"     uu$$$$$$                                                                             #",
"#                                                                   u$$$$$$$$$$$uu    \"\"\"\"\"    uuuu$$$$$$$$$$                                                                        #",
"#                                                                   $$$$\"\"\"$$$$$$$$$$uuu   uu$$$$$$$$$\"\"\"$$$\"                                                                      #",
"#                                                                    \"\"\"      \"\"$$$$$$$$$$$uu \"\"$\"\"\"                                                                            #",
"#                                                                              uuuu \"\"$$$$$$$$$$uuu                                                                                     #",
"#                                                                     u$$$uuu$$$$$$$$$uu \"\"$$$$$$$$$$$uuu$$$                                                                            #",
"#                                                                     $$$$$$$$$$\"\"\"\"           \"\"$$$$$$$$$$$\"                                                                      #",
"#                                                                    \"$$$$$\"                      \"\"$$$$\"\"                                                                          #",
"#                                                                        $$$\"                         $$$$\"                                                                             #",
"#                                                                                                                                                                                         #",
"#                                                   .S S.     sSSs_sSSs     .S       S.          .S_sSSs     .S    sSSs   .S_sSSs                                                         #",
"#                                                   .SS SS.   d%%SP~YS%%b   .SS       SS.        .SS~YS%%b   .SS   d%%SP  .SS~YS%%b                                                       #",
"#                                                   S%S S%S  d%S'     `S%b  S%S       S%S        S%S   `S%b  S%S  d%S'    S%S   `S%b                                                      #",
"#                                                   S%S S%S  S%S       S%S  S%S       S%S        S%S    S%S  S%S  S%S     S%S    S%S                                                      #",
"#                                                   S%S S%S  S&S       S&S  S&S       S&S        S%S    S&S  S&S  S&S     S%S    S&S                                                      #",
"#                                                    SS SS   S&S       S&S  S&S       S&S        S&S    S&S  S&S  S&S_Ss  S&S    S&S                                                      #",
"#                                                     S S    S&S       S&S  S&S       S&S        S&S    S&S  S&S  S&S~SP  S&S    S&S                                                      #",
"#                                                     SSS    S&S       S&S  S&S       S&S        S&S    S&S  S&S  S&S     S&S    S&S                                                      #",
"#                                                     S*S    S*b       d*S  S*b       d*S        S*S    d*S  S*S  S*b     S*S    d*S                                                      #",
"#                                                     S*S    S*S.     .S*S  S*S.     .S*S        S*S   .S*S  S*S  S*S.    S*S   .S*S                                                      #",
"#                                                     S*S     SSSbs_sdSSS    SSSbs_sdSSS         S*S_sdSSS   S*S   SSSbs  S*S_sdSSS                                                       #",
"#                                                     S*S      YSSP~YSSY      YSSP~YSSY          SSS~YSSY    S*S    YSSP  SSS~YSSY                                                        #",
"#                                                     SP                                                     SP                                                                           #",
"#                                                     Y                                                      Y                                                                            #",
"#                                                                             PRESS ANY KEY TO CONTINUE...                                                                                #",
"#                                                                                                                                                                                         #",
"#                                                                                                                                                                                         #",
"#                                                                                                                                                                                         #",
"#                                                                                                                                                                                         #",
"###########################################################################################################################################################################################"
	};
	for (int i = 0; i < 49; i++) {
		for (int j = 0; j < 188; j++) defaultBuffer[i][j] = temp[i][j];
	}
}

// 미구현
void drawItemInfo(Item* i) {
	
}
// 맵의 숫자를 문자로 바꿉니다.
char getPrintChar(int i) {
	switch (i) {
	case 0: return ' ';
	case 1: return 'W'; 
	case 2: return '.';
	case 3: return '&'; 
	case 4: return '.'; 
	case 5: return '.'; 
	case 11: //c = '└'; break;
	case 12: //c = '┘'; break;
	case 13: //c = '┌'; break;
	case 14: //c = '┐'; break;
	case 15: return '.'; //c = '┼'; break;
	case 20: return '＊'; 
	case 21: return 'S'; 
	case 22: return 'F'; 
	case 30: return 'O';
	case 31: return 'O';
	case 40: return '&';
	case 41: return '$';
	case 9: return '+'; 
	default: return (char)(i + 48); 
	}
}

// Health : 6 / 140 ~ 174 34칸 
// 175 ~ 177 / 179 ~ 181

// 플레이어 정보를 업데이트합니다.
void updatePlayerInfo(int Health, int maxHealth, int *debuff, wchar_t defaultBuffer[][189], WORD colorMap[][189], int gold, int floor, int level, int exp) {

	if (Health > 0) {
		for (int i = 0; i <= (Health) * (34 - 1) / (maxHealth); i++) {
			defaultBuffer[5][139 + i] = '=';
		}
	}
	else {
		for (int i = 0; i < 34; i++) {
			defaultBuffer[5][139 + i] = ' ';
		}
	}

	
	if (Health < 100) {
		defaultBuffer[5][175] = ' ';
	}
	else defaultBuffer[5][175] = (Health / 100) + '0';

	if (Health < 10) {
		defaultBuffer[5][176] = ' ';
	}
	else defaultBuffer[5][176] = ((Health % 100) / 10) + '0';

	defaultBuffer[5][177] = (Health % 10) + '0';

	if (maxHealth < 100) {
		defaultBuffer[5][179] = ' ';
	}
	else defaultBuffer[5][179] = (maxHealth / 100) + '0';

	if (maxHealth < 10) {
		defaultBuffer[5][180] = ' ';
	}
	else defaultBuffer[5][180] = ((maxHealth % 100) / 10) + '0';

	defaultBuffer[5][181] = (maxHealth % 10) + '0';

	wchar_t goldStr[100] = { 0, };
	char buffer[100] = { 0, };
	sprintf(buffer, "골드: %d", gold);
	mbstowcs(goldStr, buffer, strlen(buffer));

	for (int i = 139; ; i++) {
		if (goldStr[i-139] == NULL) break;
		defaultBuffer[7][i] = goldStr[i-139];
	}
	memset(buffer, 0, sizeof(buffer));
	memset(goldStr, 0, sizeof(goldStr));
	sprintf(buffer, "%d 레벨 ( %d / %d )", level, exp, MaxExp(level));
	mbstowcs(goldStr, buffer, strlen(buffer));
	for (int i = 139; ; i++) {
		if (goldStr[i - 139] == NULL) break;
		defaultBuffer[4][i] = goldStr[i - 139];
	}
	memset(buffer, 0, sizeof(buffer));
	memset(goldStr, 0, sizeof(goldStr));
	sprintf(buffer, "%2d 층", floor);
	mbstowcs(goldStr, buffer, strlen(buffer));
	for (int i = 176; ; i++) {
		if (goldStr[i - 176] == NULL) break;
		defaultBuffer[4][i] = goldStr[i - 176];
	}

	
	wchar_t str[100] = { 0, };
	char buf[100] = { 0, };
	sprintf(buf, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", (debuff[0] > 0 ? "&a치유 " : "")
		, (debuff[1] > 0 ? "&d심안 " : "")
		, (debuff[2] > 0 ? "&e정화 " : "")
		, (debuff[3] > 0 ? "&3투명 " : "")
		, (debuff[4] > 0 ? "&f도약 " : "")
		, (debuff[5] > 0 ? "&2독 " : "")
		, (debuff[6] > 0 ? "&c화상 " : "")
		, (debuff[7] > 0 ? "&e마비 " : "")
		, (debuff[8] > 0 ? "&b빙결 " : "")
		, (debuff[9] > 0 ? "&4출혈 " : "")
		, (debuff[10] > 0 ? "&8불구 " : "")
		, (debuff[11] > 0 ? "" : "")
		, (debuff[12] > 0 ? "" : "")
		, (debuff[13] > 0 ? "" : "")
		, (debuff[14] > 0 ? "" : ""));
	mbstowcs(str, buf, sizeof(str));
	int scope = 0;
	int offset = 0;
	int currentColor = 97;
	for (int i = 0; i < 2; i++) {
		offset = 0;
		for (int j = 0; j < 44; j++) {
			if (scope % 2 == offset % 2 && (str[scope] & 0x80) == 0) { // 일반 글자
				offset++;
			}

			if (j == 44 && scope % 2 == offset % 2 && (str[scope] & 0x80)) break;

			if (str[scope] == '|') {
				scope++;
				break;
			}
			if (str[scope] == NULL) return;
			// 대망의 색코드
			if (str[scope] == '&') {
				char t = str[++scope];
				if (t >= '0' && t <= '9') {
					t -= '0';
				}
				else if (t >= 'a' && t <= 'f') {
					t -= 'a' - 10;
				}

				// Minecraft Style!
				switch (t) {
				case 0: currentColor = COLOR_BLACK; break;
				case 1: currentColor = COLOR_BLUE; break;
				case 2: currentColor = COLOR_GREEN; break;
				case 3: currentColor = COLOR_CYAN; break;
				case 4: currentColor = COLOR_RED; break;
				case 5: currentColor = COLOR_MAGENTA; break;
				case 6: currentColor = COLOR_YELLOW; break;
				case 7: currentColor = COLOR_WHITE; break;
				case 8: currentColor = COLOR_BRIGHT_BLACK; break;
				case 9: currentColor = COLOR_BRIGHT_BLUE; break;
				case 10: currentColor = COLOR_BRIGHT_GREEN; break;
				case 11: currentColor = COLOR_BRIGHT_CYAN; break;
				case 12: currentColor = COLOR_BRIGHT_RED; break;
				case 13: currentColor = COLOR_BRIGHT_MAGENTA; break;
				case 14: currentColor = COLOR_BRIGHT_YELLOW; break;
				case 15: currentColor = COLOR_BRIGHT_WHITE; break;
				}

				scope++;

			}
			if (str[scope] != NULL) {
				defaultBuffer[6 + i][136 + j] = str[scope++];
				colorMap[6 + i][136 + j] = currentColor;
			}
			else break;
		}
	}
	
	
}


// (4,6) ~ (45, 129)

// 버퍼에 맵 정보를 덮어씁니다.
void updateMap(int playerX, int playerY, int renderRangeX, int renderRangeY, int map[][MIN_Y+MAX_Y], int startPosX, int endPosX, int startPosY, int endPosY, wchar_t defaultBuffer[49][189]) {
	int minRenderRangeX = playerX - renderRangeX, maxRenderRangeX = playerX + renderRangeX;
	int minRenderRangeY = playerY - renderRangeY, maxRenderRangeY = playerY + renderRangeY;
	int bufferPosX = startPosX; int bufferPosY = startPosY;
	for (int x = minRenderRangeX; x < maxRenderRangeX; x++) {
		// Y equals Horizontal
		bufferPosX++;
		bufferPosY = startPosY;
		//if (x > endPosX) continue;
		for (int y = minRenderRangeY; y < maxRenderRangeY; y++) {
			//if (y > endPosY) continue;
			if (x < 0 || y < 0 || x > MIN_X + MAX_X || y > MIN_Y + MAX_Y) defaultBuffer[bufferPosX][bufferPosY++] = ' ';
			else if (x == playerX && y == playerY) defaultBuffer[bufferPosX][bufferPosY++] = '@';
			else defaultBuffer[bufferPosX][bufferPosY++] = getPrintChar(map[x][y]);
		}
		if (bufferPosX == endPosX) break;
	}
	
}


// 테스트용
int drawTitle() {
	printf("\n\n\n\n");
	printf("                                                                            #######   ######    #######    #######      \n");
	printf("                                                                               #      #         #             #         \n");
	printf("                                                                               #      ######    #######       #         \n");
	printf("                                                                               #      #               #       #         \n");
	printf("                                                                               #      ######    #######       #         \n");
	return 0;
}


// 테스트용
void drawMenu() {
	gotoxy(90, 30);
	printf("> 게임 시작");
	gotoxy(92, 32);
	printf("게임 종료");
}
// 테스트용
void render() {
	init_();
	/*drawTitle();
	drawMenu();*/
	//defaultLayout();
	while (1) {
		Sleep(100);
	}
}
// 사전 정의된 Type에 따라 height, width의 팝업을 생성합니다.
void generatePopup(int Type, int height, int width, wchar_t buffer[][189], WORD colorMap[][189]) {

	//wchar_t horizontalSign=0, verticalSign = 0;
	//mbtowc(&horizontalSign, "─", strlen("─"));
	//mbtowc(&verticalSign, "│", strlen("│"));

	int offsetX = 0, offsetY = 0;
	if (Type == POPUP_CENTER) {
		offsetX = 49 / 2 - height / 2;
		offsetY = 189 / 2 - width / 2;
	}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || i == height - 1) {
					buffer[i+offsetX][j+offsetY] = '-';
					colorMap[i+offsetX][j+offsetY] = COLOR_BRIGHT_WHITE;
				}
				else if (j == 0 || j == width - 1) {
					buffer[i+offsetX][j+offsetY] = '|';
					colorMap[i+offsetX][j+offsetY] = COLOR_BRIGHT_WHITE;
				}
				else colorMap[i+offsetX][j+offsetY] = COLOR_BLACK;
			}	
		}
}

// 스크린을 초기화합니다. (Deprecated)
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
// 이하 미구현

int renderGMap(int BitMapMapping[][MIN_Y+MAX_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom) {
	char filename[100];
	int dpi;
	HBITMAP bitmap;
	//콘솔창의 핸들을 가져온다.
	HWND hWnd = GetConsoleWindow();

	dpi = GetDPI(hWnd); //콘솔창의 해상도 정보를 가져온다.

	//가져올 파일 이름을 지정하여 이미지 파일 정보를 가져온다.
	sprintf(filename, "medic000.bmp");
	bitmap = LoadBitmap(GetModuleHandle(NULL), IDB_BITMAP5);

	//paintMap(hWnd, dpi, bitmap, BitMapMapping, p, RenderRangeX, RenderRangeY, zoom); //콘솔창에 해상도에 맞게 이미지를 그려준다.
}

void GetBMP(HDC hdc, HDC memdc, HBITMAP image) {
	BITMAP bitmap;
	//HDC bitmapDC = CreateCompatibleDC(hdc);

	GetObject(image, sizeof(bitmap), &bitmap);
	SelectObject(memdc, image);
	//BitBlt(memdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, bitmapDC, 0, 0, SRCCOPY);

	//DeleteDC(bitmapDC);
}

int GetDPI(HWND hWnd) {
	HANDLE user32 = GetModuleHandle(TEXT("user32"));
	FARPROC func = GetProcAddress(user32, "GetDpiForWindow");
	if (func == NULL)
		return 96;
	return ((UINT(__stdcall *)(HWND))func)(hWnd);
}

void paintMap(HDC backDC, HDC memdc, HDC hdc, int bitMapMapping[][MAX_Y+MIN_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom) {



	//image를 memdc에 담아오도록 한다.


	// 콘솔창에 memdc에 들어 있는 이미지를 640*480으로 그려준다.
	// 520 360 -> 플레이어 타일 시작점 (Center 532 368)

	// 20 
	int XScope = 0, YScope = 0;
	for (int j = p->playerY - (RenderRangeY / zoom); j < p->playerY + (RenderRangeY / zoom); j++, YScope++) {
		XScope = 0;
		for (int i = p->playerX - (RenderRangeX / zoom); i < p->playerX + (RenderRangeX / zoom); i++, XScope++) {
			if (j == 0 && i == 0) {
				TransparentBlt(backDC, 56 + XScope * 16 * zoom, 40 + YScope * 16 * zoom, 16 * zoom, 16 * zoom, memdc, 16 * 41, 16 * 10, 16, 16, RGB(255, 255, 255));
				continue;
			}
			int XOffset = 5, YOffset = 0;
			switch (bitMapMapping[i][j]) {
			case 0x11: XOffset = 1; YOffset = 4; break;
			case 0x12: XOffset = 0; YOffset = 3; break;
			case 0x15: XOffset = 1; YOffset = 3; break;
			case 0x16: XOffset = 2; YOffset = 3; break;
			case 0x13: XOffset = 0; YOffset = 4; break;
			case 0x14: XOffset = 0; YOffset = 5; break;
			case 0x17: XOffset = 2; YOffset = 5; break;
			case 0x101: XOffset = 21; YOffset = 4; break;
			case 0x102: XOffset = 20; YOffset = 3; break;
			case 0x105: XOffset = 22; YOffset = 3; break;
			case 0x104: XOffset = 22; YOffset = 5; break;
			case 0x103: XOffset = 20; YOffset = 5; break;
			case 0x108: XOffset = 22; YOffset = 4; break;
			case 0x109: XOffset = 20; YOffset = 4; break;
			case 0x106: XOffset = 21; YOffset = 5; break;
			case 0x107: XOffset = 21; YOffset = 3; break;
			case 0x201: XOffset = 41; YOffset = 0; break;
			case 0x202: XOffset = 42; YOffset = 0; break;

			case 0x300: XOffset = 46; YOffset = 9; break;
			case 0x301: XOffset = 48; YOffset = 9; break;

			case 0x401: XOffset = 32; YOffset = 25; break;
			case 0x402: XOffset = 30; YOffset = 25; break;
			case 0x403: XOffset = 27; YOffset = 26; break;
			case 0x404: XOffset = 29; YOffset = 26; break;
			case 0x405: XOffset = 27; YOffset = 24; break;
			case 0x406: XOffset = 29; YOffset = 24; break;
			case 0x407: XOffset = 28; YOffset = 25; break;
			default: XOffset = 5; YOffset = 0; break;
			}
			TransparentBlt(backDC, 56 + XScope * 16 * zoom, 40 + YScope * 16 * zoom, 16 * zoom, 16 * zoom, memdc, 16 * XOffset, 16 * YOffset, 16, 16, RGB(255, 255, 255));
		}
	}

	BitBlt(hdc, 0, 0, 6400, 4800, backDC, 0, 0, SRCCOPY);

	/*DeleteDC(memdc);
	DeleteDC(backDC);
	DeleteObject(bitmap);
	DeleteObject(SelectObject(backDC, backBitmap));
	ReleaseDC(hWnd, hdc);*/
}



void Render2DMap(int bitMapMapping[][MAX_Y + MIN_Y], Player* p, int RenderRangeX, int RenderRangeY, int zoom)
{
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

	int XScope = 0, YScope = 0;
	for (int j = p->playerY - (RenderRangeY / zoom); j <= p->playerY + (RenderRangeY / zoom); j++, YScope++) {
		XScope = 0;
		for (int i = p->playerX - (RenderRangeX / zoom); i <= p->playerX + (RenderRangeX / zoom); i++, XScope++) {
			if (j == p->playerY && i == p->playerX) {
				TransparentBlt(hBackDC, XScope * 16 * zoom, YScope * 16 * zoom, 16 * zoom, 16 * zoom, hMemDC, 16 * 41, 16 * 10, 16, 16, RGB(255, 255, 255));
				continue;
			}
			int XOffset = 5, YOffset = 0;
			if (j < 0 || i < 0 || j > MIN_Y + MAX_Y || i > MIN_X + MAX_X) {
				XOffset = 9;
				YOffset = 0;
			}
			else{
				switch (bitMapMapping[i][j]) {
				case 0x11: XOffset = 1; YOffset = 4; break;
				case 0x12: XOffset = 0; YOffset = 3; break;
				case 0x15: XOffset = 1; YOffset = 3; break;
				case 0x16: XOffset = 2; YOffset = 3; break;
				case 0x13: XOffset = 0; YOffset = 4; break;
				case 0x14: XOffset = 0; YOffset = 5; break;
				case 0x17: XOffset = 2; YOffset = 5; break;
				case 0x101: XOffset = 21; YOffset = 4; break;
				case 0x102: XOffset = 20; YOffset = 3; break;
				case 0x105: XOffset = 22; YOffset = 3; break;
				case 0x104: XOffset = 22; YOffset = 5; break;
				case 0x103: XOffset = 20; YOffset = 5; break;
				case 0x108: XOffset = 22; YOffset = 4; break;
				case 0x109: XOffset = 20; YOffset = 4; break;
				case 0x106: XOffset = 21; YOffset = 5; break;
				case 0x107: XOffset = 21; YOffset = 3; break;
				case 0x201: XOffset = 41; YOffset = 0; break;
				case 0x202: XOffset = 42; YOffset = 0; break;

				case 0x300: XOffset = 46; YOffset = 9; break;
				case 0x301: XOffset = 48; YOffset = 9; break;

				case 0x401: XOffset = 32; YOffset = 25; break;
				case 0x402: XOffset = 30; YOffset = 25; break;
				case 0x403: XOffset = 27; YOffset = 26; break;
				case 0x404: XOffset = 29; YOffset = 26; break;
				case 0x405: XOffset = 27; YOffset = 24; break;
				case 0x406: XOffset = 29; YOffset = 24; break;
				case 0x407: XOffset = 28; YOffset = 25; break;
				default: XOffset = 9; YOffset = 0; break;
				}
			}
			// 56 40
			TransparentBlt(hBackDC, XScope * 16 * zoom, YScope * 16 * zoom, 16 * zoom, 16 * zoom, hMemDC, 16 * XOffset, 16 * YOffset, 16, 16, RGB(255, 255, 255));
		}
		// hMemDC의 이미지를 hBackDC의 원하는 위치로 고속복사시킨다.(출력하려는 이미지를 후면버퍼에 복사시킨다.)
		// hBackDC(후면 버퍼)의 완성된 그림을 화면으로 고속복사시킨다.
		// Pixel 수가 좀 이상함. 1215 825
		
	}
	StretchBlt(hDC, 56, 40, 1014 * GetDPI(hWnd) / 100, 698 * GetDPI(hWnd) / 100, hBackDC, 0, 0, (XScope+1)*16, (YScope+1)*16, SRCCOPY);

	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}

// 컬러코드를 정의합니다.
int returnColor(int t) {
	int currentColor;
	switch (t) {
	case 0: currentColor = COLOR_BLACK; break;
	case 1: currentColor = COLOR_BLUE; break;
	case 2: currentColor = COLOR_GREEN; break;
	case 3: currentColor = COLOR_CYAN; break;
	case 4: currentColor = COLOR_RED; break;
	case 5: currentColor = COLOR_MAGENTA; break;
	case 6: currentColor = COLOR_YELLOW; break;
	case 7: currentColor = COLOR_WHITE; break;
	case 8: currentColor = COLOR_BRIGHT_BLACK; break;
	case 9: currentColor = COLOR_BRIGHT_BLUE; break;
	case 10: currentColor = COLOR_BRIGHT_GREEN; break;
	case 11: currentColor = COLOR_BRIGHT_CYAN; break;
	case 12: currentColor = COLOR_BRIGHT_RED; break;
	case 13: currentColor = COLOR_BRIGHT_MAGENTA; break;
	case 14: currentColor = COLOR_BRIGHT_YELLOW; break;
	case 15: currentColor = COLOR_BRIGHT_WHITE; break;
	}
	return currentColor;
}
