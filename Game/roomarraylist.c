#include "roomarraylist.h"
#include "room.h"
#include <stdlib.h>
/* ArrayList.c 와 함수의 기능이 완전히 같습니다. */
RoomArrayList* initRoomArray() {
	RoomArrayList* r = (RoomArrayList *) malloc(sizeof(struct RoomArrayList));
	r->size = 0;
	return r;
}

void putRoom(RoomArrayList* arr, Room* value) {
	arr->data[arr->size++] = value;
}

void removeRoom(RoomArrayList* arr, Room* index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
}

Room* getRoom(RoomArrayList* arr, int idx) {
	return arr->data[idx];
}

bool isRoomArrayEmpty(RoomArrayList* arr) {
	return arr->size == 0;
}