#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "card.h"

CARD *
create_deck ()
{
	CARD *p_deck, *p_p;
	int i, j;
	p_deck = (CARD *)calloc (52, sizeof (CARD));
	p_p = p_deck;

	for (i = HEARTS; i <= SPADES; i++)
	{
		for (j = ACE; j <= KING; j++)
		{
			p_p->suit = (enum suit)i;
			p_p->face = (enum face)j;
			p_p->isOpen = false;
			p_p++;
		}
	}

	return p_deck;	
}

void
free_deck (CARD *p_deck)
{
	free (p_deck);
	return;
}

CARD *
shuffle (CARD *p_deck, int size)
{
	int i, r;
	CARD t;
	for (i = size; i > 0; i--)
	{
		r = rand() % i;
		t = p_deck[r];
		p_deck[r] = p_deck[i - 1];
		p_deck[i - 1] = t;
	}

	return p_deck;
}

void
print_card (CARD *p_c)
{
	char face[] = "A23456789TJQK";
	char suit[] = "hcds";

	if (!p_c) return;

	if (p_c->isOpen)
	{
		attron(A_BOLD | COLOR_PAIR(p_c->suit % 2 + 2));
		printw ("%c%c", suit[p_c->suit], face[p_c->face]);
		attroff(A_BOLD | COLOR_PAIR(p_c->suit % 2 + 2));
	}
	else
	{
		attron(COLOR_PAIR(1));
		printw ("[]");
		attroff(COLOR_PAIR(1));
	}
	return;
}