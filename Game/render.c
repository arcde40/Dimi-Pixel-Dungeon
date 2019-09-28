#include <stdio.h> 
#include <Windows.h>
#include "map.h"
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

void defaultLayout(char buffer[][189]) {
	char temp[49][188] = {
		"###########################################################################################################################################################################################",
"#                                                                                                                                                                                         #",
"#   #############################################################################################################################                     TEST TESTT ESTTEST                  #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #                                                         #",
"#   #                                                                                                                           #     HP: [==================================] 100/100    #",
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
"#   #############################################################################################################################                                                         #",
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
	case 3: return '.'; 
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
// (4,6) ~ (45, 129)
void updateMap(int playerX, int playerY, int renderRangeX, int renderRangeY, int map[][MIN_Y+MAX_Y], int startPosX, int endPosX, int startPosY, int endPosY, char defaultBuffer[49][189]) {
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
		if (bufferPosX == endPosX) return;
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

