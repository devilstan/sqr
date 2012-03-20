#ifndef __QDR_STACK__
#define __QDR_STACK__

#include "qdr_private.h"

//LargestRectangle用の簡易スタック

struct Rectangle {
	int height;
	int pos;
};

struct LRStack {
	//int allocated;	//簡易対応なので、動的拡張はしない
	int count;
	struct Rectangle r[QDR_MAXMSIZE];
};

void stack_init(struct LRStack *s);
int stack_empty(struct LRStack *s);
void stack_pop(struct LRStack *s);
void stack_push(struct LRStack *s, struct Rectangle *r);
struct Rectangle *stack_top(struct LRStack *s);
int stack_size(struct LRStack *s);
void stack_dump(struct LRStack *s);


#endif
