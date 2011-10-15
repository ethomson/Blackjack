// Blackjack Simulation : Card
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
// This is a simple card object, it contains a denomination (2-10,
// Jack, Queen, King, Ace) and a suit (Spade, Club, Heart, Diamond).
// Realistically, the suit is completely unnecessary in BlackJack,
// as is the denomation of Jack, Queen, King as they could be counted
// as 10s.  But this class could potentially be reused for a different
// card game and the Blackjack game could be extended for other casino
// rules.  An example of one Blackjack "extension" is at the Westward Ho
// Casino in Las Vegas: it allows a side bet paying 2:1 that your first
// two cards will be of the same suit.  One might wish to analyze this
// strategy.

#ifndef CARD_H
#define CARD_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"

// forward declaration to avoid circular dependencies
class Shoe;

class Card
{
	public:
		Card(const char suit, const char rank);

		// weight accessor functions
		void setWeight(const int weight);
		const int getWeight();

		// rank / suit accessor functions
		const char getSuit();
		const char getRank();

		// score is like rank, but returns 'T' (ten) for any ten or face
		const char getScore();

		// list of ranks and suits, defined in Card.cpp
		// we use a static member variable to avoid defining really
		// ugly macros
		static const char suitList[];
		static const uint8_t suitCount;

		static const char rankList[];
		static const uint8_t rankCount;


	private:
		char _suit;			// suit of the card ('S' = spades, etc.)
		char _rank;			// rank of the card ('2', '3', 'A', etc.)
		int _weight;		// weight for shuffling, see Shoe::shuffle()
};

#endif // CARD_H
