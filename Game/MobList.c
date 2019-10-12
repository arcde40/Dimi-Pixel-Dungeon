#include "MobList.h"


MobList* initMobList() {
	MobList* r = (MobList *)malloc(sizeof(struct MobList));
	r->size = 0;
	return r;
}

bool isMobListEmpty(MobList* array) {
	return array->size == 0;
}

void setMobInfo(MobList* arr, int idx, MobInfo* value) {
	if (idx >= arr->size) return;
	arr->data[idx] = value;
}

void putMobInfo(MobList* arr, MobInfo* value) {
	arr->data[arr->size++] = value;
}

void removeMobInfo(MobList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

MobInfo* getMobInfo(MobList* arr, int idx) {
	return arr->data[idx];
}