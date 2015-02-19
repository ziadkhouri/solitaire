#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

#include "card.h"
#include "stack.h"

char key_map[] = {"1234qwertyuas"};
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
	HO
};

STACK board[13];

void
print_board ()
{	
	erase ();
	move (0, 0);
	print_stack (&board[O1]);
	move (1, 0);
	print_stack (&board[O2]);
	move (2, 0);
	print_stack (&board[O3]);
	move (3, 0);
	print_stack (&board[O4]);
	move (4, 0);
	printw ("------");
	move (5, 0);
	print_stack (&board[S1]);
	move (6, 0);
	print_stack (&board[S2]);
	move (7, 0);
	print_stack (&board[S3]);
	move (8, 0);
	print_stack (&board[S4]);
	move (9, 0);
	print_stack (&board[S5]);
	move (10, 0);
	print_stack (&board[S6]);
	move (11, 0);
	print_stack (&board[S7]);
	move (12, 0);
	printw ("------");
	move (13, 0);
	print_stack (&board[HC]);
	move (14, 0);
	print_stack (&board[HO]);
	move (15, 0);
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
	p_s = peek (p_src);
	p_d = peek (p_dst);

	if ((*p_dst->validate)(p_d, p_s))
	{
		pop (p_src);
		push (p_dst, p_s);
		p_s = peek (p_src);
		if (p_s)
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
clear_board ()
{
	int i;
	memset (&board, 0, sizeof (board));

	board[S1].validate = &diff_colour_desc;
	board[S2].validate = &diff_colour_desc;
	board[S3].validate = &diff_colour_desc;
	board[S4].validate = &diff_colour_desc;
	board[S5].validate = &diff_colour_desc;
	board[S6].validate = &diff_colour_desc;
	board[S7].validate = &diff_colour_desc;

	board[O1].validate = &same_suit_asc;
	board[O2].validate = &same_suit_asc;
	board[O3].validate = &same_suit_asc;
	board[O4].validate = &same_suit_asc;

	board[HC].validate = &always_false;
	board[HO].validate = &always_false;

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

	char c;

	time_t t;
	srand ((unsigned) time(&t));

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
				p_src->isSelected = false;
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
						p_src->isSelected = true;
					}
					else
					{
						p_dst = p_cur;
						if (p_dst == p_src
							|| play(p_dst, p_src))
						{
							p_src->isSelected = false;
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