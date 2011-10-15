// Blackjack Simulation : StrategyVegasDealer
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
// This is a player object which mimics the behavior of a vegas
// dealer.  It is meant primarily to correspond to the dealer at the
// table, but could be used by any player at the table for their
// strategy.  The strategy is stand on soft 17.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "StrategyVegasDealer.h"
#include "Strategy.h"
#include "Card.h"
#include "Hand.h"

// getBuyin: returns amount to buyin at table sit-down and when out of
//           funds
uint16_t StrategyVegasDealer::getBuyin()
{
	return 100;
}

// getBet: returns the amount to bet on a single hand
uint8_t StrategyVegasDealer::getBet(uint16_t bank, uint16_t cardsLeft)
{
	// suppress unused warnings
	bank = bank;
	cardsLeft = cardsLeft;

	return 1;
}

// play: play for the dealer
PlayAction StrategyVegasDealer::play(LinkedList<Card *> *cardList)
{
	// stand on soft 17
	if(Hand::getScore(cardList) >= 17)
		return Stand;

	// hit on anything lower
	return Hit;
}

// play: play for a player
PlayAction StrategyVegasDealer::play(LinkedList<Card *> *cardList,
 LinkedList<Card *> *dealerCards)
{
	// suppress unused variable warnings
	dealerCards = dealerCards;

	return play(cardList);
}

// insure: returns true if player wants insurance
bool StrategyVegasDealer::insure(LinkedList<Card *> *dealerCards,
 LinkedList<Card *> *cardList)
{
	// ignore warnings about unused variable
	dealerCards = dealerCards;
	cardList = cardList;

	// dealer strategy never insures -- (dealer cannot insure anyway)
	return false;
}

