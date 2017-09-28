// Blackjack Simulation : StrategyDealerHitsSoft17
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
// This is a strategy object which mimics the behavior of a vegas
// dealer at a cheap table, where a dealer hits on a soft 17.

#ifndef STRATEGYDEALERHITSSOFT17_H
#define STRATEGYDEALERHITSSOFT17_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Player.h"
#include "Card.h"
#include "Strategy.h"

class StrategyDealerHitsSoft17 : public Strategy
{
	public:
		uint16_t getBuyin();
		uint8_t getBet(uint16_t bank, uint16_t cardsLeft);
		PlayAction play(LinkedList<Card *> *cardList);
		PlayAction play(LinkedList<Card *> *cardList,
		 LinkedList<Card *> *dealerCards);
		bool insure(LinkedList<Card *> *dealerCards,
		 LinkedList<Card *> *cardList);

	private:
};

#endif // STRATEGYDEALERHITSSOFT17_H
