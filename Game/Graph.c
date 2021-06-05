#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>



#define	ABS(x)			( ((x)<0)?-(x):(x) )
#define SIZE 101
#define RANGE 50



// 이 소스는 테스트용이니 가뿐하게 넘어가주세요.




int main() {
	char table[10][10] = { 0, };
	
	int x1, y1, x2, y2;
	printf("첫번째 좌표를 입력하세요: ");
	scanf("%d %d", &x1, &y1);
	printf("두번째 좌표를 입력하세요: ");
	scanf("%d %d", &x2, &y2);
	printf("\n");

	for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++) table[i][j] = '.';

	table[x1][y1] = '@';
	table[x2][y2] = '@';

	if (ABS(y2 - y1) < ABS(x2 - x1)) {

		// Drawing ( -1 <= M <= 1 ) Line
		int dx = x2 - x1;
		int dy = ABS(y2 - y1);

		int yi = (y2 - y1 > 0) ? 1 : -1;

		int D = 2 * dy - dx;
		int y = y1;

		for (int x = x1; x < x2; x++) {
			table[x][y] = 'X';
			if (D > 0) {
				y += yi;
				D -= dx + dx;
			}
			D += dy + dy;
		}
	}
	else {
		// Drawing ( -1 <= M <= 1 ) Line
		int dx = ABS(x2 - x1);
		int dy = y2 - y1;

		int xi = (x2 - x1 > 0) ? 1 : -1;

		int D = 2 * dx - dy;
		int x = x1;

		for (int y = y1; y < y2; y++) {
			table[x][y] = 'X';
			if (D > 0) {
				x += xi;
				D -= dy + dy;
			}
			D += dx + dx;
			
		}
	}




	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			printf("%c", table[j][i]);
		}
		printf("\n");
	}
	
}

void plotCircle1(int x, int y, char paper[][SIZE]) {
	paper[RANGE+x][RANGE + y] = 'X';
	paper[RANGE + x][RANGE + -y] = 'X';
	paper[RANGE + -x][RANGE + y] = 'X';
	paper[RANGE + -x][RANGE + -y] = 'X';
	paper[RANGE + y][RANGE + x] = 'X';
	paper[RANGE + y][RANGE + -x] = 'X';
	paper[RANGE + -y][RANGE + x] = 'X';
	paper[RANGE + -y][RANGE + -x] = 'X';
}

int max1(int* p1, int *p2, int* q1, int* q2) {
	if (*p1 > *q1) {
		int t = *q1;
		*q1 = *p1;
		*p1 = t;
		t = *q2;
		*q2 = *p2;
		*p2 = t;
	}
}

void plotLine1(int x1, int y1, int x2, int y2, char table[][SIZE]) {
	if (ABS(y2 - y1) < ABS(x2 - x1)) {
		max1(&x1, &y1, &x2, &y2);
		// Drawing ( -1 <= M <= 1 ) Line
		int dx = x2 - x1;
		int dy = ABS(y2 - y1);

		int yi = (y2 - y1 > 0) ? 1 : -1;

		int D = 2 * dy - dx;
		int y = y1;

		for (int x = x1; x < x2; x++) {
			table[x][y] = 'X';
			if (D > 0) {
				y += yi;
				D -= dx + dx;
			}
			D += dy + dy;
		}
	}
	else {
		// Drawing ( -1 <= M <= 1 ) Line
		max1(&y1, &x1, &y2, &x2);
		int dx = ABS(x2 - x1);
		int dy = y2 - y1;

		int xi = (x2 - x1 > 0) ? 1 : -1;

		int D = 2 * dx - dy;
		int x = x1;

		for (int y = y1; y < y2; y++) {
			table[x][y] = 'X';
			if (D > 0) {
				x += xi;
				D -= dy + dy;
			}
			D += dx + dx;

		}
	}
}

void drawCircle1() {
	// For circle Drawing (It's easy!)
	char paper[SIZE][SIZE] = { 0, };
	for (int i = 0; i < SIZE; i++) for (int j = 0; j < SIZE; j++) paper[i][j] = '.';
	int R;
	scanf("%d", &R);

	int x = 0, y = R;
	plotCircle1(x, y, paper);
	// E(first) = 2*xp + 3 = 2 * 0 + 3 = 3
	int deltaE = 3;
	// SE(first) = 2*xp - 2 *yp + 5 = 2 * 0 - 2 * R + 5 = 2 * R + 5
	int deltaSE = -2 * R + 5;

	// 판별식
	int D = 1 - R;
	while (y > x) { // 2번째 Octant까지 : 1번째 Octant로 넘어가면 y > x가 됨.
		if (D < 0) { // Midpoint가 원 안의 점 (E 선택)
			D += deltaE; // 판별식에 deltaE 증가
			deltaE += 2; // deltaE = Ep+1 - Ep = (2*(xp+1) + 3) - (2*xp + 3) = 2xp + 2 + 3 - 2*xp - 3 = 2
			deltaSE += 4; // deltaSE = SEp+1 - SEp = (2*xp+1 - 2*yp+1 + 5) - (2*xp - 2yp + 5) = 2*xp + 2 - 2*yp + 2 + 5 - 2*xp +2*yp - 5 = 4
		}
		else { // MidPoint가 원 위나 원 밖의 점 (SE 선택)
			D += deltaSE; 
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		plotCircle1(x, y, paper);
		plotLine1(RANGE, RANGE, x+RANGE, y+RANGE, paper);
		plotLine1(RANGE, RANGE, -x + RANGE, y + RANGE, paper);
		plotLine1(RANGE, RANGE, x + RANGE, -y + RANGE, paper);
		plotLine1(RANGE, RANGE, -x + RANGE, -y + RANGE, paper);
		plotLine1(RANGE, RANGE, y + RANGE, x + RANGE, paper);
		plotLine1(RANGE, RANGE, y + RANGE, -x + RANGE, paper);
		plotLine1(RANGE, RANGE, -y + RANGE, x + RANGE, paper);
		plotLine1(RANGE, RANGE, -y + RANGE, -x + RANGE, paper);
	}

	for (int i = SIZE-1; i >= 0; i--) {
		for (int j = 0; j < SIZE; j++) {
			printf("%c", paper[j][i]);
		}
		printf("\n");
	}

 }