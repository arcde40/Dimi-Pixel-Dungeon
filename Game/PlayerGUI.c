#define _CRT_SECURE_NO_WARNINGS
#include "PlayerGUI.h"

// Y: 5 ~ 47 (Incorrect)
// X: 57 ~

void generateInventory(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int cursorPosition) {
	generatePopup(POPUP_CENTER, 40, 75, defaultBuffer, colorMap);
	generatePopup(POPUP_CENTER, 40, 1, defaultBuffer, colorMap);
	for (int i = 0; i < 16; i++) {
		if (p->inventory[i] != NULL) {
			Item item = *p->inventory[i];
			wchar_t str[100] = { 0, };
			mbstowcs(str,  item.name , strlen(item.name));
			for (int j = 60; str[j-60] != NULL; j++) {
				defaultBuffer[8 + i * 2][j] = str[j-60];
				colorMap[8 + i * 2][j] = 97;
			}
		}
		else break;
	}
	wchar_t star[3] = { 0, };
	mbstowcs(star, "¢º", strlen("¢º"));
	defaultBuffer[8 + cursorPosition * 2][57] = star[0];
	defaultBuffer[8 + cursorPosition * 2][58] = star[1];
	defaultBuffer[8 + cursorPosition * 2][59] = star[2];
	colorMap[8 + cursorPosition * 2][57] = 97;
	colorMap[8 + cursorPosition * 2][58] = 97;
	colorMap[8 + cursorPosition * 2][59] = 97;
}