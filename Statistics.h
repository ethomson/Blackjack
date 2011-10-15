// Blackjack Simulation : Stats
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
// This is the statistics class, which keeps track of wins/losses
// for each (initially dealt) hand combination.  This is useful for
// tracking how well your strategy is working, whether you create a
// new strategy or are playing interactively to test your memory of
// a strategy.

#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Card.h"
#include "Hand.h"


// empty declaration to avoid circular dependencies
class Player;


class Statistics
{
	public:
		Statistics();

		// writes the outcome of a hand to the statistics array
		void write(LinkedList<Card *> *dealerCards,
		 LinkedList<Card *> *playerCards, HandOutcome result);

		// dump statistics to a file (or console)
		void dump(Player *player, FILE *fptr = stdout);

		~Statistics();

	private:
		uint8_t getScore(Card *card);

		// dump hand statistics by player card1 / player card 2
		void dumpHand(uint8_t card1, uint8_t card2, FILE *fptr = stdout);

		// the statistics array by dealer card, player cards, outcome.
		// eg:  Dealer has 9 showing, player has 4/4 and player Lost:
		// _stats[9][4][4][Loss]
		uint16_t _stats[12][22][12][3];
};

#endif // STATISTICS_H
