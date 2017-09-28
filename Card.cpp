// Blackjack Simulation : Card
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <exception>
#include <stdexcept>
#include "Util.h"
#include "Assert.h"
#include "Card.h"

// these should really be defined in Card.h, except that VisualStudio
// 6 won't let you in-place define static const members
// see http://support.microsoft.com/default.aspx?scid=kb;EN-US;241569
const uint8_t Card::suitCount = 4;
const uint8_t Card::rankCount = 13;

// define the ranks, suits of cards in a deck
const char Card::suitList[] = { 'S', 'H', 'C', 'D' };
const char Card::rankList[] = { 'A', '2', '3', '4', '5', '6', '7', '8',
	'9', 'T', 'J', 'Q', 'K' };


Card::Card(const char suit, const char rank)
{
	bool suitOk = false, rankOk = false;

	// test input for validity
	for(int i = 0; i < suitCount; i++)
	{
		if(suit == suitList[i])
			suitOk = true;
	}

	for(int j = 0; j < rankCount; j++)
	{
		if(rank == rankList[j])
			rankOk = true;
	}

	// throw exceptions on invalid input
	if(suitOk == false)
		throw std::invalid_argument("unknown suit");

	if(rankOk == false)
		throw std::invalid_argument("unknown rank");

	_suit = suit;
	_rank = rank;
}

// setWeight: weight accessor function
void Card::setWeight(const int weight)
{
	_weight = weight;
}

// getWeight: weight accessor function
const int Card::getWeight()
{
	return _weight;
}

// getSuit: suit accessor function
const char Card::getSuit()
{
	return _suit;
}

// getRank: rank accessor function
const char Card::getRank()
{
	return _rank;
}

// getScore: like getRank(), but returns 'T' (ten) for any ten or face card
const char Card::getScore()
{
	if(_rank == 'T' || _rank == 'J' || _rank == 'Q' || _rank == 'K')
		return 'T';
	else
		return _rank;
}

