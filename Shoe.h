// Blackjack Simulation : Shoe
// Copyright (c) 2004, Ed Thomson <ethomson@ravecomm.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.  See the included file 'COPYING'
// for more information.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// This is the "shoe" - which holds a number of decks of cards
// (typically 4-6) which are dealt from.  When a BlackJack table is
// opened, all decks are shuffled, the deck is cut by a player and
// then a "stopper" card is placed in the deck approximately 2 decks
// from the end.  The deal will stop and the shoe reshuffled when this
// card is reached.  This class will also hold the discard pile.
// We simulate the "stopper" card by picking a random number between
// 104-156 (ie, 2 to 3 decks from the end) and stopping when there
// are that many cards left in the draw pile.

#ifndef SHOE_H
#define SHOE_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "Card.h"

class Shoe
{
	public:
		Shoe(uint8_t numdecks = 6);

		// shuffle the shoe
		void shuffle();

		// if the "stopper" card has been reached, it's time to shuffle
		// the shoe
		bool stopperReached();

		// pull a single card out of the top of the deck and return it
		Card *deal();

		// number of cards left in the shoe
		uint16_t cardsLeft();

		~Shoe();

	private:
		bool _initialized;		// setup() has been called
		Card **_cards;			// pointer to all cards, shuffled
		Card **_draw;			// pointer to the draw cards
		uint8_t _numdecks;		// number of decks in the shoe
		uint16_t _numcards;		// number of cards in shoe
		uint16_t _numdealt;		// number of cards dealt so far
		uint16_t _stopper;		// number of cards away from end to stop
};

// ShoeShuffleCompare is suitable for passing to qsort() and compares
// the weights of each card
int ShoeShuffleCompare(const void *card1, const void *card2);

#endif // SHOE_H
