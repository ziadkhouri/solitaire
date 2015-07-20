#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <locale.h>

#include "card.h"
#include "stack.h"

char key_map[] = {"qwer1234567as"};
enum keys
{
	O1 = 0,
	O2,
	O3,
	O4,
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	HC,
	HO,
	TT 		//hidden. used for moving piles
};

STACK board[14];

void
print_board ()
{	
	erase ();
	move (0, 0);
	print_stack (&board[S1]);
	move (1, 0);
	print_stack (&board[S2]);
	move (2, 0);
	print_stack (&board[S3]);
	move (3, 0);
	print_stack (&board[S4]);
	move (4, 0);
	print_stack (&board[S5]);
	move (5, 0);
	print_stack (&board[S6]);
	move (6, 0);
	print_stack (&board[S7]);
	move (7, 0);
	printw ("------");
	move (8, 0);
	print_stack (&board[O1]);
	move (8, 8);
	print_stack (&board[O2]);
	move (8, 16);
	print_stack (&board[O3]);
	move (8, 24);
	print_stack (&board[O4]);
	move (9, 0);
	printw ("------");

	move (10, 0);
	print_stack (&board[HC]);
	move (11, 0);
	print_stack (&board[HO]);
	move (12, 0);
	printw ("");
	return;
}

void
deal (CARD *p_deck)
{
	int i, c;

	c = 0;
	for (i = 0; i < 7; i++)
		push (&board[S1], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;
	for (i = 0; i < 6; i++)
		push (&board[S2], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;
	for (i = 0; i < 5; i++)
		push (&board[S3], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;
	for (i = 0; i < 4; i++)
		push (&board[S4], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;
	for (i = 0; i < 3; i++)
		push (&board[S5], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;
	for (i = 0; i < 2; i++)
		push (&board[S6], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;

	push (&board[S7], &p_deck[c++]);
	p_deck[c - 1].isOpen = true;

	while (c < 52)
		push (&board[HC], &p_deck[c++]);

	return;
}


bool
diff_colour_desc (CARD *p_d, CARD *p_s)
{
	if (!p_s)
		return false;
	
	if (!p_d)
		return p_s->face == KING;
	
	return (p_d->suit % 2) != (p_s->suit % 2) 
		&& p_d->face == p_s->face + 1;
}

bool
same_suit_asc (CARD *p_d, CARD *p_s)
{	
	if (!p_s)
		return false;

	if (!p_d)
		return p_s->face == ACE;
	
	return p_d->suit == p_s->suit
		&& p_s->face == p_d->face + 1;
}

bool
always_false (CARD *p_d, CARD *p_s)
{
	return false;
}

void
turn ()
{
	CARD *p_c, *p_t;
	p_c = pop (&board[HC]);
	if (!p_c)
	{
		p_t = pop (&board[HO]);
		while (p_t)
		{
			p_t->isOpen = false;
			push (&board[HC], p_t);
			p_t = pop (&board[HO]);
		}
	}
	else
	{
		if ((p_t = peek (&board[HO])))
			p_t->isOpen = false;

		p_c->isOpen = true;
		push (&board[HO], p_c);
	}

	return;
}

bool
play (STACK *p_dst, STACK *p_src)
{
	CARD *p_s, *p_d;
	p_s = selected (p_src);
	p_d = peek (p_dst);

	if ((*p_dst->action)(p_d, p_s))
	{			
		if (p_d
			&& p_dst->isPile)
				p_d->isOpen = false;

		while (p_src->top >= p_src->selected
				&& (p_s = pop(p_src))
			)
		{
			if (p_dst->isPile)
				p_s->isOpen = false;
			push (&board[TT], p_s);
		}

		while ((p_d = pop (&board[TT])))
			push (p_dst, p_d);

		if ((p_d = peek (p_dst)))
			p_d->isOpen = true;

		if ((p_s = peek (p_src)))
			p_s->isOpen = true;
		return true;
	}

	return false;
}

STACK *
map (char c)
{	
	char *o = strchr (key_map, c);
	if (o != '\0')
		return &board[o - key_map];

	return NULL;
}

void 
clear_board ()
{
	int i;
	memset (&board, 0, sizeof (board));

	board[S1].action = &diff_colour_desc;
	board[S2].action = &diff_colour_desc;
	board[S3].action = &diff_colour_desc;
	board[S4].action = &diff_colour_desc;
	board[S5].action = &diff_colour_desc;
	board[S6].action = &diff_colour_desc;
	board[S7].action = &diff_colour_desc;

	board[O1].action = &same_suit_asc;
	board[O2].action = &same_suit_asc;
	board[O3].action = &same_suit_asc;
	board[O4].action = &same_suit_asc;

	board[HC].action = &always_false;
	board[HO].action = &always_false;

	board[O1].isPile = true;
	board[O2].isPile = true;
	board[O3].isPile = true;
	board[O4].isPile = true;

	for (i = 0; i < 13; i++)
		board[i].key = key_map[i];

	return;
}

int 
main (int argc, char *argv[])
{
	CARD *p_deck;
	STACK *p_cur = NULL;
	STACK *p_src = NULL;
	STACK *p_dst = NULL;

	char c = ' ';

	time_t t;
	srand ((unsigned) time (&t));
	setlocale (LC_ALL, "");

	initscr ();
	start_color ();
	cbreak ();
	init_pair (1, COLOR_CYAN, COLOR_BLACK);
	init_pair (2, COLOR_RED, COLOR_WHITE);
	init_pair (3, COLOR_BLACK, COLOR_WHITE);

	clear_board ();

	p_deck = create_deck ();
	shuffle (p_deck, 52);
	deal (p_deck);

	print_board ();
	refresh ();

	while (c != 27)
	{
		c = getch ();
		switch (c)
		{
			case 'a':
			if (p_src)
			{
				unselect (p_src);
				p_src = NULL;
			}
			turn ();
			break;

			default:
				if ((p_cur = map (c)))
				{
					if (!p_src)
					{
						p_src = p_cur;
						select (p_src);
					}
					else
					{
						p_dst = p_cur;
						if (p_dst == p_src)
						{
							select (p_src);
							//select in stack
							//(push to ptmp)
						}
						else
						{
							play(p_dst, p_src);
							unselect (p_src);
							p_src = NULL;
						}
					}
				}
				break;
		}

		print_board	();
		refresh ();
	}

	free_deck (p_deck);
	endwin ();

	return 0;
}