#include "arraylist.h"
#include "room.h"
#include <stdbool.h>
#include <stdlib.h>

Room* init(int idx, int weight, int height, int centerX, int centerY, int corridorCount) {
	Room* r = (Room*) malloc(sizeof(struct Room));
	r->idx = idx;
	r->weight = weight;
	r->height = height;
	r->centerX = centerX;
	r->centerY = centerY;
	r->corridorCount = corridorCount;
	r->connected = initArrayList();
	return r;
}

bool intersect(Room* x, Room* y) {
	if ((x->centerX <= y->centerX && y->centerX <= x->centerX + x->height) || (y->centerX <= x->centerX && x->centerX <= y->centerX + y->height))
		if ((x->centerY <= y->centerY && y->centerY <= x->centerY + x->weight) || (y->centerY <= x->centerY && x->centerY <= y->centerY + y->weight))
			return true;
	return false;
}
