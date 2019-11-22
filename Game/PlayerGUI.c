#define _CRT_SECURE_NO_WARNINGS
#include "PlayerGUI.h"

// Y: 8 ~ 47 (Incorrect)
// X: 57 ~

void generateInventory(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int cursorPosition, int buttonCurPosition) {
	generatePopup(POPUP_CENTER, 40, 75, defaultBuffer, colorMap);
	generatePopup(POPUP_CENTER, 40, 1, defaultBuffer, colorMap);
	for (int i = 0; i < p->inventory->size; i++) {
		if (i < p->inventory->size) {
			int offset = 60;
			if (cursorPosition == i) {
				wchar_t star[4] = { 0, };
				mbstowcs(star, "▶", strlen("▶"));
				for (int j = offset; star[j - offset] != NULL; j++) {
					defaultBuffer[8 + i * 2][j] = star[j - offset];
					if(buttonCurPosition == 0) colorMap[8 + i * 2][j] = 97;
					else colorMap[8 + i * 2][j] = 90;
				}
				offset += 3;
			}
			Item item = *getItem(p->inventory, i);
			wchar_t str[100] = { 0, };
			char t[100] = { 0, };
			if(item.amount == 1) sprintf(t, "%s", item.name);

			else sprintf(t, "%s X %d", item.name, item.amount);
			mbstowcs(str,  t , strlen(t));
			for (int j = offset; str[j-offset] != NULL; j++) {
				defaultBuffer[8 + i * 2][j] = str[j-offset];
				if(!(item.metadata & ITEM_IDENTIFIED)) colorMap[8 + i * 2][j] = COLOR_MAGENTA;
				else colorMap[8 + i * 2][j] = 97;
		
			}
		}
		else continue;
	}
	if (cursorPosition >= 0 && cursorPosition < p->inventory->size) {
		int line = 0;
		Item item = *getItem(p->inventory, cursorPosition);
		wchar_t str[1000] = { 0, };
		char temp[1000] = { 0, };
		if (item.ITEM_TYPE == ITEM_WEAPON) sprintf(temp, "%s|||이 무기는 &e%d레벨 무기&f이며,|&e%d - %d 데미지&f를 입힐 수 있습니다.|%s%s%s%s%s", item.lore, item.level, item.minDamage, item.maxDamage, (item.accuarcy < DEFAULT_ACCURACY) ? "이 무기는 정확도가 떨어집니다.|" : "", (item.armor > 0) ? "이 무기는 일정 피해를 흡수할 수 있습니다.|" : "", (item.attackRange > 1)? "이 무기는 멀리 떨어진 적을 공격할 수 있습니다.|" : "", (item.attackSpeed < 1) ? "이 무기는 조금 더 빠르게 공격할 수 있습니다.|":"", (!(item.metadata & ITEM_IDENTIFIED) ? "|이 무기는 감정되지 않았습니다." : ""));
		else if(item.ITEM_TYPE == ITEM_ARMOR) sprintf(temp, "%s|||이 방어구는 &e%d레벨 방어구&f이며,|&e%d - %d 데미지&f를 방어 할 수 있습니다.|%s%s", item.lore, item.level, item.minDamage, item.maxDamage, "");
		else if (item.ITEM_TYPE == ITEM_POTION) sprintf(temp, "%s|%s", item.lore, (!(item.metadata & ITEM_IDENTIFIED) ? "|&8이 물약은 감정되지 않았습니다." : ""));
		else sprintf(temp, "%s", item.lore);
		mbstowcs(str, temp, strlen(temp));
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
					line = 96 + j;
				}
				else break;
			}
		}
		

		// Print Button

		

		// 96 ~ 131 ( +35 )

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 10; k++) {
					if ((j != 0 && j != 4) && (k != 0 && k != 9)) continue;
					defaultBuffer[37+j][96 + 10 * i + k + (2 * i)] = '#';
					if (buttonCurPosition == i+1) colorMap[37 + j][96 + 10 * i + k + (2 * i)] = 93;
					else colorMap[37 + j][96 + 10 * i + k + (2 * i)] = 97;
				}
			}
		}

		// 96 108 120
		wchar_t str_1[100] = { 0, };
		if (item.ITEM_TYPE == ITEM_WEAPON  || item.ITEM_TYPE == ITEM_ARMOR || item.ITEM_TYPE == ITEM_ACCESORY) {
			mbstowcs(str_1, " 착용 ", strlen(" 착용 "));
			for (int i = 0; i < 10; i++) {
				if (str_1[i] != 0) {
					defaultBuffer[39][98+i] = str_1[i];
					if(buttonCurPosition == 1) colorMap[39][98+i] = 93;
					else colorMap[39][98 + i] = 97;
				}
			}
		}
		else {
			mbstowcs(str_1, " 사용 ", strlen(" 사용 "));
			for (int i = 0; i < 10; i++) {
				if (str_1[i] != 0) {
					defaultBuffer[39][98 + i] = str_1[i];
					if(buttonCurPosition == 1) colorMap[39][98 + i] = 93;
					else colorMap[39][98 + i] = 97;
				}
			}
		}
		wchar_t str_2[100] = { 0, };
		mbstowcs(str_2, "버리기", strlen("버리기"));
		for (int i = 0; i < 10; i++) {
			if (str_2[i] != 0) {
				defaultBuffer[39][122 + i] = str_2[i];
				if (buttonCurPosition == 3) colorMap[39][122 + i] = 93;
				else colorMap[39][122 + i] = 97;
			}
		}

	}

}

void generateSkillWindow(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int skillCursorPosition) {
	generatePopup(POPUP_CENTER, 40, 80, defaultBuffer, colorMap);

}

void generateIndexWindow(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int CategoryPosition, int subCategoryPosition) {

}