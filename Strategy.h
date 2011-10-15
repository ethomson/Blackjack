// Blackjack Simulation : Strategy
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
// This is a generic strategy object.  It is pure virtual, and must
// be overloaded by a strategy object which implements the playing
// logic - either an interactive player which prompts for the action,
// or a piece of code which examines the hand and plays.
//
// Note: some functions are really only for strategy objects which
// implement User Interfaces.  (Example: summarize.)  Those need
// not be implemented by inheriting classes.

#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Card.h"
#include "Hand.h"

typedef enum { None, Stand, Hit, Double, Split } PlayAction;


class Strategy
{
	public:
		// determine how much to buyin at
		virtual uint16_t getBuyin() = 0;

		// get the bet for the next play - bet may change for
		// interactive strategies
		virtual uint8_t getBet(uint16_t bank, uint16_t cardsLeft) = 0;

		// get play action (first decl is for the dealer)
		virtual PlayAction play(LinkedList<Card *> *cardList) = 0;
		virtual PlayAction play(LinkedList<Card *> *cardList,
		 LinkedList<Card *> *dealerCards) = 0;

		// offer insurance
		virtual bool insure(LinkedList<Card *> *dealerCards,
		 LinkedList<Card *> *cardList) = 0;

		// notify of blackjacks - useful for interactive strategies
		// or maybe for keeping statistics when counting cards
		virtual void notifyDealerBlackjack(void) { }
		virtual void notifyPlayerBlackjack(void) { }

		// notify the player of a shuffle - useful for card counting
		// strategies to zero the count index
		virtual void notifyShuffle(void) { }

		// summarize the hand at the end of play - useful for
		// interactive strategies
		virtual void summarize(LinkedList<Card *> *dealerCards,
		 LinkedList<Hand *> *hands)
		{ dealerCards = dealerCards; hands = hands; }
};

#endif // STRATEGY_H
