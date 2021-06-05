#include <stdbool.h>
#include <stdlib.h>
#include "arraylist.h"

// ArrayList 援ъ“泥대 Initialize⑸땲
ArrayList* initArrayList() {
	ArrayList* r = (ArrayList*) malloc(sizeof(struct ArrayList));
	r->size = 0;
	for (int i = 0; i < 100; i++) r->data[i] = 0;
	return r;
}
// ArrayList대떦 媛믪쓣 吏묒뼱ｌ뒿덈떎.
void put(ArrayList* arr, int value) {
	arr->data[arr->size++] = value;
}
// ArrayListindex踰덉㎏ 媛믪쓣 놁빋덈떎.
void removeContent(ArrayList* arr, int index) {
	for (int i = index; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
}
// ArrayListidx踰덉㎏ 媛믪쓣 媛몄샃덈떎.
int get(ArrayList* arr, int idx) {
	return arr->data[idx];
}
// list덈뒗 紐⑤뱺 媛믪쓣 target0踰덉㎏ 媛믩異붽⑸땲
void addAll(ArrayList* target, ArrayList* list) {
	for (int i = 0; i < list->size; i++) {
		if (containsContent(target,list->data[i])) continue;
		put(target, get(list,i));
	}
}
// 대떦 媛믪쓣 ArrayList媛 媛吏怨덈뒗吏 뚯븘遊낅땲
bool containsContent(ArrayList* target, int content) {
	int idx = -1;
	for (int i = 0; i < target->size; i++) if (target->data[i] == content) idx = i;
	return idx != -1;
}

// ArrayList꾨Т 媛믩룄 ㅼ뼱덉 딆吏瑜諛섑솚⑸땲
bool isEmpty(ArrayList* arr) {
	return arr->size == 0;
}