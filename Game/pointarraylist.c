#include <stdbool.h>
#include "pointarraylist.h"
PointArrayList* initPointArray() {
	PointArrayList* r = (PointArrayList *)malloc(sizeof(struct PointArrayList));
	r->size = 0;
	return r;
}

bool isPointArrayListEmpty(PointArrayList* array) {
	return array->size == 0;
}

void setPoint(PointArrayList* arr, int idx, POINT_P* value) {
	if (idx >= arr->size) return;
	arr->data[idx] = value;
}

void putPoint(PointArrayList* arr, POINT_P* value) {
	arr->data[arr->size++] = value;
}

void removePoint(PointArrayList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

POINT_P* getPoint(PointArrayList* arr, int idx) {
	if (arr == NULL) return NULL;
	return arr->data[idx];
}