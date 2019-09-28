#include <stdbool.h>
#include <stdlib.h>
#include "arraylist.h"

ArrayList* initArrayList() {
	ArrayList* r = (ArrayList*) malloc(sizeof(struct ArrayList));
	r->size = 0;
	for (int i = 0; i < 100; i++) r->data[i] = 0;
	return r;
}

void put(ArrayList* arr, int value) {
	arr->data[arr->size++] = value;
}

void removeContent(ArrayList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
}

int get(ArrayList* arr, int idx) {
	return arr->data[idx];
}

void addAll(ArrayList* target, ArrayList* list) {
	for (int i = 0; i < list->size; i++) {
		if (containsContent(target,list->data[i])) continue;
		put(target, get(list,i));
	}
}

bool containsContent(ArrayList* target, int content) {
	int idx = -1;
	for (int i = 0; i < target->size; i++) if (target->data[i] == content) idx = i;
	return idx != -1;
}

bool isEmpty(ArrayList* arr) {
	return arr->size == 0;
}