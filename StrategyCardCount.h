// Blackjack Simulation : StrategyCardCount
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
// This is the basic blackjack strategy as defined at
// http://wizardofodds.com/games/blackjack.

#ifndef STRATEGYCARDCOUNT_H
#define STRATEGYCARDCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Player.h"
#include "Card.h"
#include "Strategy.h"


class StrategyCardCount : public Strategy
{
	public:
		StrategyCardCount();
		uint16_t getBuyin();
		uint8_t getBet(uint16_t bank, uint16_t cardsLeft);
		void notifyShuffle(void);
		PlayAction play(LinkedList<Card *> *cardList);
		PlayAction play(LinkedList<Card *> *cardList,
		 LinkedList<Card *> *dealerCards);
		bool insure(LinkedList<Card *> *dealerCards,
		 LinkedList<Card *> *cardList);
		void summarize(LinkedList<Card *> *dealerCards,
		 LinkedList<Hand *> *hands);

	private:
		void updateWeight(Card *card);
		float getTrueCount(uint16_t cardsLeft = 0);

		uint8_t _bet;
		int32_t _weight;
		uint16_t _cardsLeft;
};

#endif // STRATEGYCARDCOUNT_H
