#include <stdbool.h>
#include "ItemArrayList.h"

ItemArrayList* initItemArray() {
	ItemArrayList* r = (ItemArrayList *)malloc(sizeof(struct ItemArrayList));
	r->size = 0;
	return r;
}

bool isItemArrayListEmpty(ItemArrayList* array) {
	return array->size == 0;
}

void setItem(ItemArrayList* arr, int idx, Item* value) {
	if (idx >= arr->size) return;
	arr->data[idx] = value;
}

void putItem(ItemArrayList* arr, Item* value) {
	for (int i = 0; i < arr->size; i++) {
		if (getItem(arr, i)->id == value->id && value->metadata & ITEM_STACKABLE) {
			getItem(arr, i)->amount++;
			return;
		}
	}
	value->amount++;
	arr->data[arr->size++] = value;
}

void removeItem(ItemArrayList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

Item* getItem(ItemArrayList* arr, int idx) {
	if (arr == NULL) return NULL;
	if (idx >= arr->size) return NULL;
	return arr->data[idx];
}