#pragma once
#include "struct.h"
#include <stdbool.h>
typedef struct PointArrayList {
	int size;
	POINT_P* data[10000];
}PointArrayList;

PointArrayList* initPointArray();
void putPoint(PointArrayList* arr, POINT_P* value);
void removePoint(PointArrayList* arr, int index);
POINT_P* getPoint(PointArrayList* arr, int idx);
bool isPointArrayListEmpty(PointArrayList* arr);
void setPoint(PointArrayList* arr, int idx, POINT_P* value);