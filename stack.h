#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include "card.h"

typedef struct _STACK
{
	CARD *p_cards[52];
	int top;
	char key;
	int selected; //HACK: 0 = false, so idx = selected- 1
	bool isPile;
	bool (*action) (CARD *, CARD *);
} STACK;

void push (STACK *p_s, CARD *p_c);
CARD *pop (STACK *p_s);
CARD *peek (STACK* p_s);
CARD *select (STACK* p_s);
CARD *selected (STACK* p_s);
void unselect (STACK* p_s);
void print_stack (STACK *p_s);

#endif