// Blackjack Simulation : Table
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
// This is a simple table object, which contains the shoe, a dealer
// and one or more players.

#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Shoe.h"
#include "Player.h"
#include "Strategy.h"
#include "Card.h"

class Table
{
	public:
		Table();

		// add a player to the table (with name of strategy)
		void addPlayer(const char *strategyType);

		// loop - does all the work of taking bets, dealing cards,
		// paying in/out, etc.
		void loop(void);

		// clear the bets and remove all the cards
		void clearBets();

		// shuffle the deck and notify players of a shuffle
		void shuffle(void);

		// get bets from the players
		void getBets();

		// deal two cards to each player (and the dealer)
		void deal();

		// offer insurance when dealer shows ace
		void offerInsurance();

		// handle dealer Blackjack
		void dealerBlackjack(Card *dealerShowCard);

		// check players for Blackjack and pay out
		void checkBlackjack();

		// handle play from each player
		void play();

		// handle a play action (hit, stand, double, etc) for a player
		// and for the dealer
		bool handleAction(PlayAction action, Player *player, Hand *hand);
		bool handleAction(PlayAction action, LinkedList<Card *> *cards);

		// determine winners and take bets / pay out bets at the end of
		// each hand
		void payInOut();

		// write statistics on wins / pushes / losses for each player
		void writeStats();

		// summarize will tell each player the details of the hand
		void summarize();

		~Table();

	private:
		Shoe *_shoe;							// the shoe (card decks)
		LinkedList<Player *> *_players;			// list of players
		uint32_t _handCount;					// # of hands played

		Strategy *_dealerStrategy;				// dealer's strategy
		LinkedList<Card *> *_dealerCards;		// dealer's current cards
		LinkedList<Card *> *_dealerShowCards;	// cards minus hidden card
};

#endif // TABLE_H
