// Blackjack Simulation : Hand
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
// This is a hand -- it contains a list of cards (ie, the hand) and
// a bet for that hand.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Card.h"
#include "Hand.h"

Hand::Hand(bool isSplit)
{
	// create a card list for this hand
	_cards = new LinkedList<Card *>;

	// setup some defaults
	_bet = 0;
	_betInsurance = false;
	_blackjack = false;
	_outcome = Push;
	_splitHand = NULL;
	_isSplit = isSplit;
}

// getCards: card accessor function
LinkedList<Card *> *Hand::getCards()
{
	return _cards;
}

// getScore: return the score of the current hand, ie 10/6 returns 16,
//           Ace/6 returns 17
uint8_t Hand::getScore()
{
	// call the static getScore function
	return getScore(_cards);
}

// getSoft: return the "softness" of the current hand, ie Ace/6 would
//          return true, Ace/10/6 would return false
bool Hand::getSoft()
{
	return getSoft(_cards);
}

// splitAllowed: returns true on a pair of the same cards (8/8 or 10/10)
//               and false on a non-splittable hand (eg 5/6)
bool Hand::splitAllowed()
{
	return splitAllowed(_cards);
}

// getScore: determines the score of the hand
uint8_t Hand::getScore(LinkedList<Card *> *cards, bool *soft)
{
	Card *card = NULL;
	uint8_t score = 0;
	uint8_t aces = 0;

	// examine each card in the hand
	while((card = cards->iterate()) != NULL)
	{
		// get the rank
		char cardRank = card->getRank();

		// face cards are worth ten points
		if(
			cardRank == 'T' || cardRank == 'J' ||
			cardRank == 'Q' || cardRank == 'K'
		)
		{
			score += 10;
		}

		// aces are worth 1 or 11 - we assume 11 for now and keep a
		// count of the number of aces - if we would bust, we'll treat
		// them as a 1 instead.
		else if(cardRank == 'A')
		{
			score += 11;
			aces++;
		}

		// otherwise, just get the score of the card
		else
		{
			// since this is a char type, its value is the ASCII value of the
			// character code.  subtracting 48 will give us this in decimal
			score += (cardRank - 48);
		}
	}

	// up to now, we counted all the aces as 11 points.  if we're over
	// 21 points and we have a "soft" hand (ie, we've used an ace) then
	// we want to instead count those aces as 1.  so as long as we're over
	// 21, we subtract 10 off for each ace in our hand.
	while(score > 21 && aces > 0)
	{
		score -= 10;
		aces--;
	}

	// assign to the soft boolean if it was provided (typically by
	// getSoft, below)
	if(soft != NULL)
		*soft = (aces > 0) ? true : false;

	return score;
}

// getSoft: returns true if the hand is a soft hand (ie, Ace/6)
bool Hand::getSoft(LinkedList<Card *> *cards)
{
	bool soft;

	// let getScore do all the work rather than reimplementing it
	getScore(cards, &soft);

	return soft;
}

// splitAllowed: returns true if this hand may be split (ie, only
//               two cards of the same rank)
bool Hand::splitAllowed(LinkedList<Card *> *cards)
{
	// can only split on the initial deal (two cards)
	if(cards->getCount() != 2)
		return false;

	// get the two cards
	Card *firstCard = cards->fetchFront();
	Card *lastCard = cards->fetchTail();

	// get the rank of the two cards
	char firstRank = firstCard->getRank();
	char lastRank = lastCard->getRank();

	// you may split tens of any type
	if(
		firstRank == 'T' || firstRank == 'J' ||
		firstRank == 'Q' || firstRank == 'K'
	)
	{
		return((
			lastRank == 'T' || lastRank == 'J' ||
			lastRank == 'Q' || lastRank == 'K'
		) ? true : false);
	}

	// otherwise, they must have the same rank
	else
		return ((firstRank == lastRank) ? true : false);
}

// setBet: set the bet for this hand (strategy sets the bet)
//         and reset the rest of the hand for play
void Hand::setBet(uint8_t bet)
{
	_bet = bet;

	// we're resetting the hand, there can't be insurance or
	// blackjack
	_betInsurance = false;
	_blackjack = false;
}

// getBet: bet accessor function
uint8_t Hand::getBet()
{
	return _bet;
}

// setInsurance: insurance accessor function
void Hand::setInsurance(bool insure)
{
	_betInsurance = insure;
}

// getInsurance: insurance accessor function
bool Hand::getInsurance()
{
	return _betInsurance;
}

// setBlackjack: blackjack accessor functions
void Hand::setBlackjack(bool bj)
{
	_blackjack = bj;
}

// getBlackjack: blackjack accessor functions
bool Hand::getBlackjack()
{
	return _blackjack;
}

// setOutcome: outcome accessor function
void Hand::setOutcome(HandOutcome outcome)
{
	_outcome = outcome;
}

// getOutcome: outcome accessor function
HandOutcome Hand::getOutcome()
{
	return _outcome;
}

// setSplitHand: splitHand accessor function
void Hand::setSplitHand(Hand *splitHand)
{
	_splitHand = splitHand;
}

// getSplitHand: splitHand accessor function
Hand *Hand::getSplitHand()
{
	return _splitHand;
}

// isSplitHand: splitHand accessor function
bool Hand::isSplitHand()
{
	return _isSplit;
}

Hand::~Hand()
{
	delete _cards;
}

