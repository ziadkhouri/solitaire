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
unselect (STACK *p_s)
{
	p_s->selected = 0;
}

CARD *
select (STACK *p_s)
{
	if (p_s->selected < 2
		|| !p_s->p_cards[p_s->selected - 2]->isOpen)
	{
		p_s->selected = p_s->top;
		return p_s->p_cards[p_s->selected - 1];
	}
	p_s->selected--;

	return p_s->p_cards[p_s->selected - 1];
}

CARD *
selected (STACK *p_s)
{
	if (!p_s->selected)
		return NULL;

	return p_s->p_cards[p_s->selected - 1];
}

void
print_stack (STACK *p_s)
{
	int i;
	printw ("%c:", p_s->key);
	if (p_s->selected)
		attron (A_BLINK);
	if (p_s->isPile)
	{
		print_card (peek (p_s));
		if (p_s->selected)
			printw ("<");
	}
	else
	{
		for (i = 0; i < p_s->top; i++)
		{
			print_card (p_s->p_cards[i]);
			if (i == p_s->selected - 1)
				printw ("<");
		}
	}
	if (p_s->selected)
	{
		attroff (A_BLINK);
	}
}