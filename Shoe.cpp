// Blackjack Simulation : Shoe
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
// This is the "shoe" - which holds a number of decks of cards
// (typically 4-6) which are dealt from.  When a BlackJack table is
// opened, all decks are shuffled, the deck is cut by a player and
// then a "stopper" card is placed in the deck approximately 2 decks
// from the end.  The deal will stop and the shoe reshuffled when this
// card is reached.  This class will also hold the discard pile.
// We simulate the "stopper" card by picking a random number between
// 104-156 (ie, 2 to 3 decks from the end) and stopping when there
// are that many cards left in the draw pile.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <exception>
#include <stdexcept>
#include "Util.h"
#include "Assert.h"
#include "Card.h"
#include "Shoe.h"

Shoe::Shoe(uint8_t numdecks)
{
	// absolute maximum number of decks is 1260 - otherwise we can't
	// hold the number of cards in the shoe in a 16-bit unsigned
	if(numdecks > 1260)
		throw std::invalid_argument("maximum number of decks is 1260");

	// setup some defaults
	_cards = NULL;
	_draw = NULL;

	_numdecks = numdecks;
	_numcards = _numdecks * Card::rankCount * Card::suitCount;
	_numdealt = 0;
	_stopper = 0;

	// seed the pseudo-random number generator with the current time
	// and the PID
	srand_portable();

	// malloc an array for the cards
	_cards = new Card*[_numcards];

	// put the cards into the shoe -- each deck gets one rank of each suit
	int cardcnt = 0;

	// put the cards into the shoe just like they do in a casino - each
	// deck comes ordered by rank and suit:

	// iterate through each deck
	for(uint8_t i = 0; i < _numdecks; i++)
	{
		// iterate through each suit
		for(uint8_t j = 0; j < Card::suitCount; j++)
		{
			// iterate through each card
			for(uint8_t k = 0; k < Card::rankCount; k++)
			{
				// and create that card
				Card *card = new Card(Card::suitList[j], Card::rankList[k]);

				_cards[cardcnt++] = card;
			}
		}
	}

	shuffle();
}

// shuffle : shuffle the deck using the method outlined by most online
//           casinos, that is for each card in the deck, assign a random
//           number ("weight") to the card.  then sort by the weight.
void Shoe::shuffle()
{
	// determine the number of cards per deck
	int cardsPerDeck = Card::rankCount * Card::suitCount;

	// assign a (pseudo-)random weight to each card in the shoe
	for(uint16_t i = 0; i < _numcards; i++)
	{
		// set the weight
		_cards[i]->setWeight(rand());
	}

	// sort the cards by weight using qsort(3)
	qsort(_cards, _numcards, sizeof(Card *), ShoeShuffleCompare);

	// set the deal point at the beginning of the card list
	_draw = _cards;

	// setup the stopper - note that some (old, poor) implementations of
	// rand(3) have the less random bits in the lower-order bits.  see
	// linux rand(3) or "Numerical Recipes in C": Flannery, et. al.
	// we want a random number between 52 and 104 (ie, 2.5 decks +/-
	// .5 deck.)
	_stopper = cardsPerDeck +
	 (int)((cardsPerDeck * 2) * (double)(rand() / (RAND_MAX + 1.0)));

	// a single card gets "burned" at the beginning of the deal
	_draw++;
	_numdealt = 1;
}

// ShoeShuffleCompare : comparison function for qsort(3).  compares the
//                      weight of two cards in a manner suitable for
//                      qsort(3).
int ShoeShuffleCompare(const void *ptr1, const void *ptr2)
{
	// whee, casting void pointers - but realistically, quite safe when
	// qsort(3) is calling us
	Card *card1 = *(Card **)ptr1;
	Card *card2 = *(Card **)ptr2;

	// get the weight of each card
	int weight1 = card1->getWeight();
	int weight2 = card2->getWeight();

	// qsort expects -1 if input1 < input2, 1 if input1 > input2,
	// 0 if input1 == input2
	if(weight1 < weight2)
		return -1;
	else if(weight1 == weight2)
		return 0;
	else
		return 1;
}

// stopperReached: returns true if we've reached the virtual "little
//                 yellow card" that tells us to stop dealing.
bool Shoe::stopperReached()
{
	if(_stopper > (_numcards - _numdealt))
		return true;

	return false;
}

// deal: return a single card from the top of the deck and increment
//       the draw pointer
Card *Shoe::deal()
{
	ASSERT(_numdealt < _numcards);

	// whee pointer arithmetic - post-increment the draw pointer after
	// dereferencing it.  ie, get the card at the draw pointer and set
	// it to the next card.
	Card *card = *(_draw++);

	_numdealt++;

	return card;
}

// cardsLeft: returns number of cards left in the shoe
uint16_t Shoe::cardsLeft()
{
	return(_numcards - _numdealt);
}

Shoe::~Shoe()
{
	// delete each card
	for(uint16_t i = 0; i < _numcards; i++)
	{
		delete _cards[i];
	}

	// and delete the card array
	delete _cards;
}
