// Blackjack Simulation : StrategyInteractive
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
// This isn't a "strategy" per se, but it still controls the playing
// from stdio.  Note: C++'s iostreams are so horrible for printing
// formatted text that we use printf.  Yes, it's possible to go too far
// with objects.

#ifndef STRATEGYINTERACTIVE_H
#define STRATEGYINTERACTIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Player.h"
#include "Card.h"
#include "Strategy.h"

class StrategyInteractive : public Strategy
{
	public:
		StrategyInteractive();
		uint16_t getBuyin();
		uint8_t getBet(uint16_t bank, uint16_t cardsLeft);
		PlayAction play(LinkedList<Card *> *cardList);
		PlayAction play(LinkedList<Card *> *cardList,
		 LinkedList<Card *> *dealerCards);
		bool insure(LinkedList<Card *> *dealerCards,
		 LinkedList<Card *> *cardList);
		void notifyDealerBlackjack(void);
		void notifyPlayerBlackjack(void);
		void notifyShuffle(void);
		void summarize(LinkedList<Card *> *dealer,
		 LinkedList<Hand *> *hands);

	private:
		char getInput(const char *prompt, char defaultInput);
		uint8_t getInput(const char *prompt, int defaultInput);

		uint8_t _bet;
};

#endif // STRATEGYINTERACTIVE_H
