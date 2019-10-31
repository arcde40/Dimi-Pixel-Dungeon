#define _CRT_SECURE_NO_WARNINGS
#include "render.h"
// X: 188 Y: 49 최대

void init_() {
	system("mode con cols=188 lines=49 | title TeST");
}


void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(consoleHandle, coord);
}

void defaultLayout(wchar_t buffer[][189]) {
	// Y: 30~46 X: 134~183
	char temp[49][188] = {
		"###########################################################################################################################################################################################",
"#                                                                                                                                                                                         #",
"#   #############################################################################################################################                     TEST TESTT ESTTEST                  #",
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
	case 40: return '&';
	case 41: return '$';
	case 9: return '+'; 
	default: return (char)(i + 48); 
	}
}

// Health : 6 / 140 ~ 174 34칸 
// 175 ~ 177 / 179 ~ 181
void updatePlayerInfo(int Health, int maxHealth, wchar_t defaultBuffer[][189]) {

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
}


// (4,6) ~ (45, 129)
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



int drawTitle() {
	printf("\n\n\n\n");
	printf("                                                                            #######   ######    #######    #######      \n");
	printf("                                                                               #      #         #             #         \n");
	printf("                                                                               #      ######    #######       #         \n");
	printf("                                                                               #      #               #       #         \n");
	printf("                                                                               #      ######    #######       #         \n");
	return 0;
}



void drawMenu() {
	gotoxy(90, 30);
	printf("> 게임 시작");
	gotoxy(92, 32);
	printf("게임 종료");
}

void render() {
	init_();
	/*drawTitle();
	drawMenu();*/
	//defaultLayout();
	while (1) {
		Sleep(100);
	}
}

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

