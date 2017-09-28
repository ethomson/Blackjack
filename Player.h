// Blackjack Simulation : Player
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
// This is a player object, which controls the way a player interacts.
// It controls the player's bet, bankroll, cards, etc.

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Strategy.h"
#include "Hand.h"
#include "Statistics.h"


class Player
{
	public:
		Player(const char *strategyName = "dealer");

		// clear the hands - ready for a new deal
		void clearHands();

		// hand accessor
		LinkedList<Hand *> *getHands(void);

		// give the player another hand - typically used in a split
		void addHand(Hand *hand);

		// get the strategy object
		Strategy *getStrategy();

		// get the statistics object
		Statistics *getStatistics();

		// return the number of hands played
		uint32_t handsPlayed();

		// buyin - we keep track of how much money is put in and then
		// put it into the bankroll
		void addBuyin(uint16_t bank);

		// add bankroll - suitable for pays to the player
		void addBankroll(uint16_t bank, uint16_t bankChange = 0);

		// remove from bankroll - when a hand is played
		void removeBankroll(uint16_t bank, uint16_t bankChange = 0);

		// get the amount of the bankroll in dollars and cents
		// we use an integer for change so that we don't deal with
		// floating point rounding, but we do need to keep track of
		// change for 3:2 blackjack payouts on odd bets (eg, 3:2 on
		// a $5 bet is a $7.50 payout)
		uint16_t getBankroll();
		uint8_t getBankrollChange();

		// get the total buyin so far
		uint32_t getBuyin();

		~Player();

	private:
		uint32_t _handsPlayed;			// number of hands played
		uint32_t _buyinTotal;			// total amount bought in at
		uint16_t _bankroll;				// current bankroll
		uint8_t _bankrollChange;		// bankroll change (in pennies)
		Strategy *_strategy;			// playing strategy (see Strategy.h)
		LinkedList<Hand *> *_hands;		// list of hands currently played
		Statistics *_statistics;		// statistics for this strategy
};

#endif // PLAYER_H
