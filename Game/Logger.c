#include "render.h"
#include "Logger.h"

// Y: 31~47 X: 133~182
// 화면 우하단에 로그를 뿌려줍니다.
void printLog(wchar_t defaultBuffer[][189], LogArrayList* str) {
	//int line = 47;
	wchar_t* t;
	t = getString(str, str->size - 1);
	bool end = true;
	// 16 -> 13
	for (int y = 13; y >= 0; y--) {

		if (end) {
			end = false;
			if (str->size <= (13-y)) return;
			t = getString(str, str->size - (13-y)-1);
		}

		for (int x = 0; x <= 49; x++) {
			if (t[x] != 0) defaultBuffer[y + 31][x + 133] = t[x];
			else {
				end = true;
				break;
			}
		}
		
	}
}