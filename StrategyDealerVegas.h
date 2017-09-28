// Blackjack Simulation : StrategyVegasDealer
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
// dealer.  It is meant primarily to correspond to the dealer at the
// table, but could be used by any player at the table for their
// strategy.  The strategy is stand on soft 17.

#ifndef STRATEGYDEALERVEGAS_H
#define STRATEGYDEALERVEGAS_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Player.h"
#include "Card.h"
#include "Strategy.h"

class StrategyDealerVegas : public Strategy
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

#endif // STRATEGYDEALERVEGAS_H
