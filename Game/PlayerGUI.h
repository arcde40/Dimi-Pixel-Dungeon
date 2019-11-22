#pragma once
#include "GlobalVariable.h"
#include "render.h"
#include <stdlib.h>
#include <Windows.h>
#include "Item.h"
#include "Player.h"
#include "ItemArrayList.h"

void generateInventory(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int cursorPosition, int buttonCurPosition);
void generateSkillWindow(Player* p, wchar_t defaultBuffer[][189], WORD colorMap[][189], int skillCursorPosition);