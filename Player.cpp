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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <exception>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Player.h"
#include "Strategy.h"
#include "StrategyLookup.h"
#include "Hand.h"
#include "Statistics.h"


Player::Player(const char *strategyName)
{
	// set defaults
	_handsPlayed = 0;
	_buyinTotal = 0;
	_bankroll = 0;
	_bankrollChange = 0;

	// get a strategy from the lookup table
	try {
		_strategy = StrategyLookup(strategyName);
	}
	catch(...) {
		throw;
	}

	// setup a linked list for the hands for this player
	try {
		_hands = new LinkedList<Hand *>();
	}
	catch(...) {
		// need to delete the strategy instantiated above to avoid memleaks
		delete _strategy;

		// rethrow exception
		throw;
	}

	// setup a statistics handler
	try {
		_statistics = new Statistics();
	}
	catch(...) {
		// cleanup to avoid memleaks
		delete _strategy;
		delete _hands;

		// rethrow exception
		throw;	
	}
}

// clearHands: delete the current hands for this player and create a
//             new one
void Player::clearHands()
{
	Hand *hand;

	_handsPlayed++;

	// delete each hand in play
	while((hand = _hands->removeTail()) != NULL)
		delete hand;

	// create a new hand
	hand = new Hand();

	// add it to the list of hands
	_hands->add(hand);
}

// getHands: hands accessor function
LinkedList<Hand *> *Player::getHands()
{
	return _hands;
}

// addHand: adds a hand to the linked list (like when a split occurs, a
//          new hand is created and then added to the hands list here)
void Player::addHand(Hand *hand)
{
	_hands->addTail(hand);
}

// getStatistics: statistics accessor function
//                returns the statistics object for this player
Statistics *Player::getStatistics()
{
	return _statistics;
}

// getStrategy: strategy accessor function
//              returns the strategy object for this player
Strategy *Player::getStrategy()
{
	return _strategy;
}

// handsPlayed: returns the number of hands played
uint32_t Player::handsPlayed()
{
	return _handsPlayed;
}

// addBuyin: lets the player "buy in" - increase their bankroll
//           useful for keeping statistics about buyin/payout percentage
void Player::addBuyin(uint16_t bank)
{
	_buyinTotal += bank;
	_bankroll += bank;
}

// addBankroll: adds to the player's bankroll when they win a hand
//              (or an insurance side bet.)  in dollars and cents.
//              we do not use a float for bankroll to avoid floating
//              point rounding errors.  it's difficult to bet
//              9.99999999 dollars, after all
void Player::addBankroll(uint16_t bank, uint16_t bankChange)
{
	ASSERT(bankChange < 100);

	_bankroll += bank;
	_bankrollChange += bankChange;

	// change in cents for dollars
	while(_bankrollChange >= 100)
	{
		_bankrollChange -= 100;
		_bankroll += 1;
	}
}

// removeBankroll: removes from a player's bankroll to bet on a hand
//                 realistically, bankChange should always be zero since
//                 the table probably won't take change.
//                 NOTE: you must do error checking in advance
void Player::removeBankroll(uint16_t bank, uint16_t bankChange)
{
	ASSERT(bankChange < 100);
	ASSERT(bank <= _bankroll);

	_bankroll -= bank;

	// hand change if they want it
	if(bankChange > 0)
	{
		_bankroll -= 1;
		_bankrollChange = 100 - bankChange;
	}
}

// getBankroll: return the current bankroll
uint16_t Player::getBankroll()
{
	return _bankroll;
}

// getBankrollChange: return the current bank change
uint8_t Player::getBankrollChange()
{
	return _bankrollChange;
}

// getBuyin: return the total buyin so far
uint32_t Player::getBuyin()
{
	return _buyinTotal;
}

Player::~Player()
{
	Hand *hand;

	// delete each hand
	while((hand = _hands->removeTail()) != NULL)
		delete hand;

	delete _strategy;
	delete _hands;
	delete _statistics;
}


