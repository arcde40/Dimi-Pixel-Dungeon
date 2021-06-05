#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <Windows.h>
#include <stdlib.h>

typedef struct LogArrayList {
	int size;
	wchar_t* data[1000];
}LogArrayList;

LogArrayList* initLogArray();
void putMBS(LogArrayList* arr, char* value);
void removeString(LogArrayList* arr, int index);
wchar_t* getString(LogArrayList* arr, int idx);
bool isLogArrayListEmpty(LogArrayList* arr);
void setMBS(LogArrayList* arr, int idx, char* value);