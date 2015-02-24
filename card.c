#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
//#include <locale.h>

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

//Fisher–Yates shuffle
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
	//char suit[] = "hcds";
	wchar_t suit[] = L"\u2665\u2660\u2666\u2663";


	if (!p_c) return;

	if (p_c->isOpen)
	{
		attron (COLOR_PAIR(p_c->suit % 2 + 2));
		printw ("%c%lc", face[p_c->face], suit[p_c->suit]);
		attroff (COLOR_PAIR(p_c->suit % 2 + 2));
	}
	else
	{
		attron (COLOR_PAIR(1));
		printw ("[]");
		attroff (COLOR_PAIR(1));
	}
	return;
}