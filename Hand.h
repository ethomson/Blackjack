// Blackjack Simulation : Hand
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
// This is a hand -- it contains a list of cards (ie, the hand) and
// a bet for that hand.

#ifndef HAND_H
#define HAND_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Card.h"


typedef enum { Loss = 0, Push = 1, Win = 2 } HandOutcome;


class Hand
{
	public:
		Hand(bool isSplit = false);
		LinkedList<Card *> *getCards();

		// get the hand score, softness (ie, ace 6 is "soft 17")
		// and whether split is allowed (ie, two matching cards)
		uint8_t getScore();
		bool getSoft();
		bool splitAllowed();

		// static versions of the same for use when you don't have
		// a hand object (like the dealer obj) just a linked list of
		// cards
		static uint8_t getScore(LinkedList<Card *> *cards,
		 bool *soft = NULL);
		static bool getSoft(LinkedList<Card *> *cards);
		static bool splitAllowed(LinkedList<Card *> *cards);

		// bet amount accessors
		void setBet(uint8_t bet);
		uint8_t getBet();

		// insurance accessor functions
		void setInsurance(bool insure = true);
		bool getInsurance();

		// blackjack accessor functions
		void setBlackjack(bool bj = true);
		bool getBlackjack();

		// outcome accessor functions
		void setOutcome(HandOutcome outcome);
		HandOutcome getOutcome();

		// split hand accessor functions
		void setSplitHand(Hand *splitHand);
		Hand *getSplitHand();

		bool isSplitHand();
		~Hand();


	private:
		LinkedList<Card *> *_cards;	// card list (the hand)
		uint8_t _bet;				// bet for this hand
		bool _betInsurance;			// this hand is insured
		bool _blackjack;			// hand is blackjack (Ace + 10 card)
		HandOutcome _outcome;		// outcome (Win, Loss, Push)
		bool _isSplit;				// hand is part of a split
		Hand *_splitHand;			// ptr to the other hand in the split
};

#endif // HAND_H
