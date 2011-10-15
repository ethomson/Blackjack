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
// http://wizardofodds.com/games/blackjack, with simple card counting
// as described at http://www.allaboutblackjack.com/cardcounting.html

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "StrategyCardCount.h"
#include "Strategy.h"
#include "Card.h"
#include "Hand.h"

StrategyCardCount::StrategyCardCount()
{
	// default bet $1
	_bet = 1;

	// count weight is zero on every deal
	_weight = 0;

	// we don't yet know how many cards are left
	_cardsLeft = 0;
}

// getBuyin: returns amount to buyin at table sit-down and when out of
//           funds
uint16_t StrategyCardCount::getBuyin()
{
	// let's buyin at $100
	return 100;
}

// getBet: returns the amount to bet on a single hand
uint8_t StrategyCardCount::getBet(uint16_t bank, uint16_t cardsLeft)
{
	// suppress unused warnings
	bank = bank;

	float trueCount = getTrueCount(cardsLeft);

	// increase bet when rich in 10s
	if(trueCount <= 1)
		return _bet;
	else if(trueCount <= 3)
		return (_bet * 2);
	else if(trueCount <= 5)
		return (_bet * 3);
	else if(trueCount <= 7)
		return (_bet * 4);
	else
		return (_bet * 5);
}

// shuffle: reset card count weight to zero
void StrategyCardCount::notifyShuffle(void)
{
	_weight = 0;
}

// play: play for the dealer - CardCount strategy can't be used for the dealer
//       so we always return 'None'
PlayAction StrategyCardCount::play(LinkedList<Card *> *cardList)
{
	// suppress unused variable warnings
	cardList = cardList;

	// interactive strategy cannot be used for dealer
	ASSERT(0);

	return None;
}

// play: play for the player
PlayAction StrategyCardCount::play(LinkedList<Card *> *cardList,
 LinkedList<Card *> *dealerCards)
{
	bool canSplit = false;
	char splitRank = 0;

	// get our score and score the dealer's showing
	uint8_t dealerScore = Hand::getScore(dealerCards);

	uint8_t playerScore = Hand::getScore(cardList);
	bool playerSoft = Hand::getSoft(cardList);

	// see if we can split this hand - two cards of matching rank
	if(cardList->getCount() == 2)
	{
		Card *first = cardList->fetchFront();
		Card *last = cardList->fetchTail();

		if(first->getScore() == last->getScore())
		{
			canSplit = true;
			splitRank = first->getScore();
		}
	}

	// SPLITTABLE HANDS

	// twos and threes get split against a weak dealer
	if(canSplit && (splitRank == '2' || splitRank == '3'))
		return ((dealerScore < 8) ? Split : Hit);

	// fours only get split against a dealer bust card (5, 6)
	else if(canSplit && splitRank == '4')
		return ((dealerScore == 5 || dealerScore == 6) ? Split : Hit);

	// treat pair of fives like a ten and double, not split

	// split sixes against a weak dealer
	else if(canSplit && splitRank == '6')
		return ((dealerScore < 7) ? Split : Hit);

	// split sevens against a weak dealer
	else if(canSplit && splitRank == '7')
		return ((dealerScore < 8) ? Split : Hit);

	// "always split aces and eights"
	else if(canSplit && splitRank == '8')
		return Split;

	// split nines against a weak dealer
	else if(canSplit && splitRank == '9')
		return ((dealerScore == 7 || dealerScore > 9) ? Stand : Split);

	// "never split a winner"
	else if(canSplit && splitRank == 'T')
		return Stand;

	// "always split aces and eights"
	else if(canSplit && splitRank == 'A')
		return Split;

	// SOFT HANDS

	// soft 13 and 14 are weak and safe to hit on
	if((playerScore == 13 || playerScore == 14) && playerSoft)
		return ((dealerScore == 5 || dealerScore == 6) ? Double : Hit);

	// soft 15 and 16 are weak and safe to hit on
	else if((playerScore == 15 || playerScore == 16) && playerSoft)
		return ((dealerScore > 3 && dealerScore < 7) ? Double : Hit);

	// we'll still hit a soft 17
	else if(playerScore == 17 && playerSoft)
		return ((dealerScore > 2 && dealerScore < 7) ? Double : Hit);

	// strong hand, but we should play aggressive against a weak
	// dealer and hit against a strong dealer
	else if(playerScore == 18 && playerSoft)
	{
		if(dealerScore > 2 && dealerScore < 7)
			return Double;
		else if(dealerScore < 9)
			return Stand;
		else
			return Hit;
	}

	// HARD HANDS AND NO PAIRS - NO ACE, NO SPLITTING

	// always hit on 8 or less
	else if(playerScore <= 8)
		return Hit;

	// 9 is strong, easy double hand
	else if(playerScore == 9)
		return ((dealerScore > 2 && dealerScore < 7) ? Double : Hit);

	// 10 is strong, great double hand
	else if(playerScore == 10)
		return ((dealerScore < 10) ? Double : Hit);

	// 11 is terribly strong, perfect doubling hand
	else if(playerScore == 11)
		return ((dealerScore < 11) ? Double : Hit);

	// 12 is an okay hand to hit on, but not unless we need to
	else if(playerScore == 12)
		return ((dealerScore > 3 && dealerScore < 7) ? Stand : Hit);

	// 13/14 is getting weaker to hit on, 15/16 very weak
	else if(playerScore > 12 && playerScore < 17)
		return ((dealerScore < 7) ? Stand : Hit);

	// stand on 17
	else if(playerScore >= 17)
		return Stand;
		

	ASSERT(0);
	return None;
}

// insure: returns true if player wants insurance
bool StrategyCardCount::insure(LinkedList<Card *> *dealerCards,
 LinkedList<Card *> *cardList)
{
	// suppress unused warnings
	dealerCards = dealerCards;
	cardList = cardList;

	// insure when the deck is rich in 10s
	float trueCount = getTrueCount();

	if(trueCount >= 1)
		return true;

	return false;
}

// summarize: sum up the card count
void StrategyCardCount::summarize(LinkedList<Card *> *dealerCards,
 LinkedList<Hand *> *hands)
{
	Card *card;
	Hand *hand;

	while((card = dealerCards->iterate()) != NULL)
		updateWeight(card);

	while((hand = hands->iterate()) != NULL)
	{
		while((card = hand->getCards()->iterate()) != NULL)
			updateWeight(card);
	}
}

// updateWeight: updates the current weight
void StrategyCardCount::updateWeight(Card *card)
{
	char rank = card->getRank();

	if(rank >= '3' && rank <= '7')
		_weight++;
	else if(rank == 'A' || card->getScore() == 'T')
		_weight--;
}

// getTrueCount: gets the "true count" - the running count divided by the
//               number of cards left.  See the card-counting description
//               at http://www.allaboutblackjack.com/cardcounting.html
float StrategyCardCount::getTrueCount(uint16_t cardsLeft)
{
	if(cardsLeft > 0)
		_cardsLeft = cardsLeft;

	float decksLeft = ((float)_cardsLeft / (float)52);
	float trueCount = ((float)_weight / decksLeft);

	return trueCount;
}
