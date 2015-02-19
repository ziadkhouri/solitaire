#ifndef _CARD_H
#define _CARD_H

#include <stdbool.h>

enum suit
{
	HEARTS = 0,
	CLUBS,
	DIAMONDS,
	SPADES
};

enum face
{
	ACE = 0,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE, 
	TEN,
	JACK,
	QUEEN,
	KING
};

typedef struct _CARD
{
	enum suit suit;
	enum face face;
	bool isOpen;
} CARD;

CARD *create_deck ();
void free_deck (CARD *p_deck);
CARD *shuffle (CARD *p_deck, int size);
void print_card (CARD *p_c);

#endif