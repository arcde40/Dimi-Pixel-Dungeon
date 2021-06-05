#ifndef ARRAY_HEADER
#define ARRAY_HEADER
#include <stdbool.h>

typedef struct ArrayList {
	int size;
	int data[100];
}ArrayList;

ArrayList* initArrayList();
void put(ArrayList* arr, int value);
void removeContent(ArrayList* arr, int index);
bool isEmpty(ArrayList* arr);
int get(ArrayList* arr, int idx);
void addAll(ArrayList* target, ArrayList* list);
bool containsContent(ArrayList* target, int content);
#endif