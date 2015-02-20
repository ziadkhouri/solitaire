#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include "card.h"

typedef struct _STACK
{
	CARD *p_cards[52];
	int top;
	char key;
	bool isSelected;
	bool isHeap;
	//function pointer valdate()
	bool (*validate) (CARD *, CARD *);
} STACK;

void push (STACK *p_s, CARD *p_c);
CARD *pop (STACK *p_s);
CARD *peek (STACK* p_s);
void print_stack (STACK *p_s);

#endif