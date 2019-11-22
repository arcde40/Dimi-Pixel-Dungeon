#pragma once
#include "Item.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct ItemArrayList {
	Item* data[100];
	int size;
}ItemArrayList;

ItemArrayList* initItemArray();
bool isItemArrayListEmpty(ItemArrayList* array);
void setItem(ItemArrayList* arr, int idx, Item* value);
void putItem(ItemArrayList* arr, Item* value);
void removeItem(ItemArrayList* arr, int index);
Item* getItem(ItemArrayList* arr, int idx);