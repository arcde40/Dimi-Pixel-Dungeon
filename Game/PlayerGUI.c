#define _CRT_SECURE_NO_WARNINGS
#include "PlayerGUI.h"

// Y: 5 ~ 47 (Incorrect)
// X: 57 ~

void generateInventory(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int cursorPosition) {
	generatePopup(POPUP_CENTER, 40, 75, defaultBuffer, colorMap);
	generatePopup(POPUP_CENTER, 40, 1, defaultBuffer, colorMap);
	for (int i = 0; i < 16; i++) {
		if (p->inventory[i] != NULL) {
			int offset = 60;
			if (cursorPosition == i) {
				wchar_t star[4] = { 0, };
				mbstowcs(star, "▶", strlen("▶"));
				for (int j = offset; star[j - offset] != NULL; j++) {
					defaultBuffer[8 + i * 2][j] = star[j - offset];
					colorMap[8 + i * 2][j] = 97;
				}
				offset += 3;
			}
			Item item = *p->inventory[i];
			wchar_t str[100] = { 0, };
			mbstowcs(str,  item.name , strlen(item.name));
			for (int j = offset; str[j-offset] != NULL; j++) {
				defaultBuffer[8 + i * 2][j] = str[j-offset];
				colorMap[8 + i * 2][j] = 97;
			}
		}
		else break;
	}
	if (cursorPosition >= 0 && p->inventory[cursorPosition] != NULL) {
		Item item = *p->inventory[cursorPosition];
		wchar_t str[1000] = { 0, };
		mbstowcs(str, item.lore, strlen(item.lore));
		int scope = 0;
		int offset = 0;
		int currentColor = 97;
		for (int i = 0; i < 30; i++) {
			offset = 0;
			for (int j = 0; j < 35; j++) {
				if (scope % 2 == offset % 2 && (str[scope] & 0x80) == 0) { // 일반 글자
					offset++;
				}

				if (j == 34 && scope % 2 == offset % 2 && (str[scope] & 0x80)) break;

				if (str[scope] == '|') {
					scope++;
					break;
				}
				// 대망의 색코드
				if (str[scope] == '&') {
					char t = str[++scope];
					if (t >= '0' && t <= '9') {
						t -= '0';
					}
					else if(t >= 'a' && t <= 'f'){
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
					defaultBuffer[8 + i][96 + j] = str[scope++];
					colorMap[8 + i][96 + j] = currentColor;

				}
				else return;
			}
		}
	}
	
}