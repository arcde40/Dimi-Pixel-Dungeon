#include "pathfinding.h"

POINT_P* pointer[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, };

void sort(PointArrayList* array, int f[][MAX_Y + MIN_Y]) {
	 int flag = 1;
	while (flag) {
		flag = 0;
		for (int i = 1; i < array->size; i++) {
			if (f[getPoint(array, i)->x][getPoint(array, i)->y] < f[getPoint(array, i - 1)->x][getPoint(array, i - 1)->y]) {
				flag = 1;
				POINT_P* point = getPoint(array, i);
				setPoint(array, i, getPoint(array, i - 1));
				setPoint(array, i - 1, point);
			}
		}
	}
}

int getH(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

void cpyMap(int target[][MIN_Y+MAX_Y], int to[][MIN_Y+MAX_Y]) {
	for (int i = 0; i < MAX_X + MIN_X; i++) {
		for (int j = 0; j < MAX_Y + MIN_Y; j++) {
			to[i][j] = target[i][j];
		}
	}
}


PointArrayList* findPath(int map[][MIN_Y + MAX_Y], int startPosX, int startPosY, int endPosX, int endPosY, MobList* mobList, int step) {
	/*
	* Debug Property
	* 40 OpenList 41 ClosedList
	*/
	int t = 0;
	//int temp[MAX_X+MIN_X][MIN_Y + MAX_Y] = { 0, };
	//cpyMap(map, temp); 
	//char tempBuffer[49][189] = { 0, };
	// Alter : changing order
	if(!isPassable(map[endPosX][endPosY])) return NULL;
	int g[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, };
	int h[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, };
	int f[MAX_X + MIN_X][MAX_Y + MIN_Y] = { 0, };
	int currentX = startPosX, currentY = startPosY;
	PointArrayList* openList = initPointArray();
	PointArrayList* closedList = initPointArray();
	g[startPosX][startPosY] = 0;
	POINT_P* point = (POINT_P*)malloc(sizeof(POINT_P));
	point->x = startPosX; point->y = startPosY;
	//point->pointX = startPosX; point->y = startPosY;
	putPoint(openList, point);
	do {
		if (step > 0 && t++ == step) break;
		sort(openList, f);
		// Exception Handling


		putPoint(closedList, getPoint(openList, 0));
		//temp[getPoint(openList, 0)->x][getPoint(openList, 0)->y] = 41;
		currentX = getPoint(openList, 0)->x; currentY = getPoint(openList, 0)->y;
		if (openList->size > 0) removePoint(openList, 0);

		if (currentX == endPosX && currentY == endPosY) break;
		//defaultLayout(tempBuffer);
		//updateMap(startPosX, startPosY, 20, 60, temp, 4, 45, 6, 129, tempBuffer);
		//for (int i = 0; i < 49; i++) {
		//	fprintf(stdout, "%s", tempBuffer[i]);
		//	if (i != 48) fprintf(stdout, "\n");
		//}
		//Sleep(50);
		//ClearScreen();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i != 0 || j != 0) {
					int t = -1;
					for (int k = 0; k < closedList->size; k++) {
						if (getPoint(closedList, k)->x == currentX + i && getPoint(closedList, k)->y == currentY + j) {
							t = k;
						}
					}
					

					if (!isOverlapping(currentX + i , currentY + j, mobList) && isPassable(map[currentX + i][currentY + j]) && t == -1) {
						if (f[currentX + i][currentY + j] == 0) {
							h[currentX + i][currentY + j] = getH(currentX + i, currentY + j, endPosX, endPosY);
							g[currentX + i][currentY + j] = g[currentX][currentY] + 1;
							// altered g[currentX][currentY]
							f[currentX + i][currentY + j] = h[currentX + i][currentY + j] + g[currentX+i][currentY+j];
							POINT_P* point2 = (POINT_P*) malloc(sizeof(POINT_P));
							point2->x = currentX+i; point2->y = currentY+j;
							point2->pointX = currentX; point2->pointY = currentY;
							putPoint(openList, point2);
							//temp[currentX + i][currentY + j] = 40;
						}
						else {
							int idx = -1;
							for (int k = 0; k < openList->size; k++) {
								if (getPoint(openList, k)->x == currentX + i && getPoint(openList, k)->y == currentY + j) {
									idx = k;
								}
							}
							if (idx >= 0) {
								if (g[currentX + i][currentY + j] > g[currentX][currentY] + 1) {
									g[currentX + i][currentY + j] = g[currentX][currentY] + 1;
									// altered h[currentX][currentY]
									f[currentX + i][currentY + j] = g[currentX + i][currentY + j] + h[currentX + i][currentY + j];
									getPoint(openList, idx)->pointX = currentX;
									getPoint(openList, idx)->pointY = currentY;
									/*POINT_P* point3 = (POINT_P*)malloc(sizeof(POINT_P));
									point3->x = currentX + i; point3->y = currentY + j;
									//pointer[currentX + i][currentY + j] = point3;
									point3->pointX = currentX; point3->pointY = currentY;*/
								}
							}
						}
					}
					else continue;
				}
			}
		}
		
	} while (openList->size > 0);

	if (isPointArrayListEmpty(openList)) {
		free(closedList);
		free(openList);
		return NULL;
	}
	else {
		// return closedList;
		// Inversing!
		PointArrayList* inversed = initPointArray();
		int currentX = 0, currentY = 0;
		int findX = 0, findY = 0;
		int scope = closedList->size-1;
		while (currentX != startPosX || currentY != startPosY) {
			POINT_P* t = getPoint(closedList, scope);
			findX = t->pointX;
			findY = t->pointY;
			for (int i = closedList->size - 1; i >= 0; i--) {
				if (getPoint(closedList, i)->x == findX && getPoint(closedList, i)->y == findY) {
					scope = i;
					break;
				}
			}
			putPoint(inversed, t);
			currentX = t->x; currentY = t->y;
		}
		free(closedList);
		free(openList);
		return inversed;
	}
}