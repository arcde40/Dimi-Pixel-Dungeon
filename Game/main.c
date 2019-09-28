#include <stdio.h>
#include "room.h"
#include "roomarraylist.h"
#include "map.h"
#include "render.h"
#include "struct.h"
#include "pathfining.h"
#include "InputHandler.h"
#include <time.h>
#include <Windows.h>

#include "GlobalVariable.h"


int main() {
	srand(time(NULL));
	int map[MIN_X+MAX_X][MIN_Y+MAX_Y] = { {0,} };
	RoomArrayList* array = initRoomArray();
	for (int i = 0; i < MIN_ROOM_COUNT || (i < MAX_ROOM_COUNT && (int)(rand() * 10 % 2) == 0); i++) {
		putRoom(array, init(i, (int)(rand() * 10) % (MAX_ROOM_WEIGHT - MIN_ROOM_WEIGHT) + MIN_ROOM_WEIGHT, (int)(rand() * 10) % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT) + MIN_ROOM_HEIGHT, (int)(rand() * 10) % (MAX_X - MIN_X) + MIN_X, (int)(rand() * 10) % (MAX_Y - MIN_Y) + MIN_Y, 0));
	}
	draw(map, array);


	int playerX = getStartPos().X, playerY = getStartPos().Y;
	init_();
	char defaultBuffer[49][189] = { 0, };
	defaultLayout(defaultBuffer);
	for (int i = 0; i < 49; i++) {
		fprintf(stdout, "%s\n",defaultBuffer[i]);
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

	// ���콺 Ȱ��ȭ
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	
	while (1) {
		
		Sleep(MAX_FRAME);
		//ClearScreen();
		//system("cls");
		fflush(stdout);
		
		defaultLayout(defaultBuffer);
		gotoxy(0, 0);
		updateMap(playerX, playerY, 20, 60, map, 4, 45, 6, 129, defaultBuffer);
		
		//Sleep(100);
		for (int i = 0; i < 49; i++) {
			fprintf(stdout, "%s", defaultBuffer[i]);
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

					path = findPath(map, playerX, playerY, playerX + x - 25, playerY + y - 66);

					if (path != NULL) {
						pathSequence = path->size - 1;
						isMoving = true;
					}
				}
			}
		}

		if (isMoving) {
			playerX = getPoint(path, pathSequence)->x;
			playerY = getPoint(path, pathSequence--)->y;
			if (pathSequence == -1) isMoving = false;
		}

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