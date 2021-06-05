#include "LogArrayList.h"
/* ArrayList.c 와 함수의 기능이 완전히 같습니다. */
LogArrayList* initLogArray() {
	LogArrayList* r = (LogArrayList *)malloc(sizeof(struct LogArrayList));
	r->size = 0;
	return r;
}

bool isLogArrayListEmpty(LogArrayList* array) {
	return array->size == 0;
}

void setMBS(LogArrayList* arr, int idx, char* value) {
	if (idx >= arr->size) return;
	mbstowcs(arr->data[idx],value,strlen(value));
}

void putMBS(LogArrayList* arr, char* value) {
	arr->data[arr->size] = (wchar_t*) malloc(sizeof(wchar_t) * 512);
	memset(arr->data[arr->size], '\0', sizeof(wchar_t)*512);

	mbstowcs(arr->data[arr->size++], value, strlen(value));
}

void removeString(LogArrayList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

wchar_t* getString(LogArrayList* arr, int idx) {
	return arr->data[idx];
}