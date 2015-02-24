#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "stack.h"

void
push (STACK *p_s, CARD *p_c)
{
	if (p_s->top < 52)
	{
		p_s->p_cards[p_s->top] = p_c;
		p_s->top++;
	}
	return;
}

CARD *
pop (STACK *p_s)
{
	if (p_s->top > 0)
	{
		p_s->top--;
		return p_s->p_cards[p_s->top];
	} 
	return NULL;
}

CARD *
peek (STACK* p_s)
{
	if (p_s->top > 0)
	{
		return p_s->p_cards[p_s->top - 1];
	} 
	return NULL;
}

void
print_stack (STACK *p_s)
{
	int i;
	printw ("%c:", p_s->key);
	if (p_s->isSelected)
		attron (A_BLINK);
	if (p_s->isPile)
	{
		print_card (peek (p_s));
	}
	else
	{
		for (i = 0; i < p_s->top; i++)
			print_card (p_s->p_cards[i]);
	}
	if (p_s->isSelected)
	{
		attroff (A_BLINK);
		printw (" <");
	}
}