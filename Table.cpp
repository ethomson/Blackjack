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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Table.h"
#include "Shoe.h"
#include "Strategy.h"
#include "StrategyLookup.h"
#include "StrategyDealerVegas.h"
#include "StrategyInteractive.h"

Table::Table()
{
	// create the shoe for the table
	try {
		_shoe = new Shoe();
	} catch(...) {
		// rethrow exceptions
		throw;
	}

	// create the dealer object
	_dealerStrategy = new StrategyDealerVegas();
	_dealerCards = new LinkedList<Card *>();
	_dealerShowCards = new LinkedList<Card *>();

	// create the player list
	_players = new LinkedList<Player *>();

	_handCount = 0;
}

// addPlayer: adds a player with a given strategy to the table
void Table::addPlayer(const char *strategyType)
{
	Player *player;
	Strategy *strategy;

	// create the player
	try
	{
		player = new Player(strategyType);
	}
	catch(...)
	{
		throw;
	}

	// get a buyin for the player, and give them chips ("bankroll")
	strategy = player->getStrategy();
	player->addBuyin(strategy->getBuyin());

	// add the player to the end of the player list
	_players->addTail(player);

	return;
}

// loop: plays hands - we basically keep playing hands until the
//       player sends SIGINTR (^C or ^Break)
void Table::loop(void)
{
	while(1)
	{
		// each player is only allowed to play one hand at a time.
		// the easiest way to do this is just to destroy their old
		// hands and require new bets
		clearBets();

		// clear everybody's hand
		_dealerCards->clear();
		_dealerShowCards->clear();

		// check to see if we've reached the "stopper" in the shoe
		// and need to shuffle
		if(_shoe->stopperReached())
			shuffle();

		// require bets
		getBets();

		// deal the cards
		deal();

		// check for ace showing on dealer's hand (the first card is "hidden",
		// so we only examine the last)  if the dealer has an ace showing,
		// we offer players insurance
		if(_dealerShowCards->fetchFront()->getRank() == 'A')
		{
			// offer insurance
			offerInsurance();
		}

		// check for blackjack
		if(Hand::getScore(_dealerCards) == 21)
		{
			// dealer has blackjack
			dealerBlackjack(_dealerShowCards->fetchFront());
		}
		else
		{
			// check for players blackjack, pay 3:2
			checkBlackjack();

			// let the players, then the dealer play
			play();

			// pay out the winning players, take money from losing players
			payInOut();
		}

		// write statistics
		writeStats();

		// display a summary of the hand
		summarize();
	}
}

// clearBets: clears bets and cards from in front of each player
void Table::clearBets()
{
	Player *player;

	// walk through the player list, clear their hands and their
	// bets
	while((player = _players->iterate()) != NULL)
		player->clearHands();
}

// shuffle: shuffle the shoe, notify players of the shuffle
void Table::shuffle()
{
	Player *player;

	// shuffle the shoe
	_shoe->shuffle();

	// notify the players that a shuffle has occured
	while((player = _players->iterate()) != NULL)
	{
		Strategy *strategy = player->getStrategy();
		strategy->notifyShuffle();
	}
}

// getBets: get the bet from each player, take it out of their bankroll
void Table::getBets()
{
	Player *player;
	Hand *hand;
	uint8_t bet;

	// walk through the player list
	while((player = _players->iterate()) != NULL)
	{
		Strategy *strategy = player->getStrategy();
		LinkedList<Hand *> *handList = player->getHands();
		uint8_t tryBet = 0;

		bet = 0;
		while(bet == 0)
		{
			// we only try 3 times to get a bet, in case the strategy
			// is poorly implemented, to avoid getting caught in an
			// infinite loop
			if(tryBet > 3)
			{
				fprintf(stderr, "Cannot obtain proper bet amount.\n");
				abort();
			}

			// examine their bankroll, make sure they have enough to
			// bet.  if not, allow them to attempt to buy back in
			uint16_t bankroll = player->getBankroll();
			bet = strategy->getBet(bankroll, _shoe->cardsLeft());

			if(bet > bankroll)
			{
				uint16_t buyin;

				// if they want to bet more than their bankroll, offer
				// them a larger bankroll
				buyin = strategy->getBuyin();

				// add to their bankroll
				if(buyin > 0)
					player->addBuyin(buyin);

				// they still don't have enough, let them try again
				if(bet > player->getBankroll())
					bet = 0;
			}

			// they exceeded the maximum bet for the table
			else if(bet > 100)
			{
				printf("The maximum bet is $100 per hand.\n");
				bet = 0;
			}

			// they didn't enter a bet
			else if(bet == 0)
			{
				uint8_t maxBet = (bankroll < 100) ? bankroll : 100;
				printf("Please enter a whole number between 0-%d.\n", maxBet);
			}

			tryBet++;
		}

		// each hand needs cards -- a player could be playing
		// multiple hands
		while((hand = handList->iterate()) != NULL)
		{
			// set their bet for the hand and remove the amount from
			// their bankroll
			hand->setBet(bet);
			player->removeBankroll(bet);
		}
	}
}

// deal: deal two cards to the dealer and each player
void Table::deal()
{
	Player *player;
	Hand *hand;

	// deal cards - once around the table starting with the first
	// player and ending with the dealer, then a second card to
	// each player
	for(int i = 0; i < 2; i++)
	{
		// deal each player their card
		while((player = _players->iterate()) != NULL)
		{
			LinkedList<Hand *> *handList = player->getHands();

			// each hand needs cards -- a player could be playing
			// multiple hands
			while((hand = handList->iterate()) != NULL)
			{
				// and deal the cards
				LinkedList<Card *> *cardList = hand->getCards();
				cardList->addTail(_shoe->deal());
			}
		}

		// give a card to the dealer -- get his hand and append a card to it
		Card *dealerCard = _shoe->deal();

		_dealerCards->addTail(dealerCard);

		// only put the last card in the dealer's "show" list -- this is
		// what is passed to players to let them determine how to bet
		if(i > 0)
			_dealerShowCards->addTail(dealerCard);
	}
}

// offerInsurance: when the dealer has an 'Ace' showing, we offer the
//                 players a side bet called 'insurance' - the bet pays
//                 2:1 if the dealer has blackjack, otherwise the player
//                 loses their insurance bet and plays the hand out
void Table::offerInsurance()
{
	Player *player;

	// offer insurance to each player
	while((player = _players->iterate()) != NULL)
	{
		Hand *hand;
		Strategy *strategy = player->getStrategy();

		// take from each hand
		LinkedList<Hand *> *handList = player->getHands();

		// offer insurance to each hand of the player
		while((hand = handList->iterate()) != NULL)
		{
			// offer insurance
			if(strategy->insure(_dealerShowCards, hand->getCards()))
				hand->setInsurance();
		}
	}
}

// dealerBlackjack: when the dealer has BlackJack, pay out insurance
//                  bets and take any original bets (all bets lose on
//                  dealer blackjack, except player blackjack when
//                  the dealer had a 10 showing - that's a push)
void Table::dealerBlackjack(Card *dealerShowCard)
{
	Player *player;

	// take money from each player
	while((player = _players->iterate()) != NULL)
	{
		Hand *hand;
		Strategy *strategy = player->getStrategy();

		// take from each hand
		LinkedList<Hand *> *handList = player->getHands();

		while((hand = handList->iterate()) != NULL)
		{
			// get the score from the player, make sure they didn't
			// have blackjack - that's a special case
			uint8_t handScore = hand->getScore();

			// a push occurs when the dealer shows a ten -- if the dealer
			// shows an ACE, the player must have taken insurance for even
			// money, otherwise they lose their bet on dealer blackjack
			if(handScore == 21 && dealerShowCard->getRank() == 'A')
			{
				// pay insurance (even money)
				if(hand->getInsurance())
					player->addBankroll(hand->getBet());

				hand->setOutcome(Loss);
			}

			// push - player had blackjack and dealer didn't have the
			// ace showing
			else if(handScore == 21)
			{
				// pay back original bet (ie, push)
				player->addBankroll(hand->getBet());

				hand->setOutcome(Push);
			}

			// player loses their bet, but insurance bet gets paid
			else
			{
				// pay insurance
				if(hand->getInsurance())
					player->addBankroll(hand->getBet());

				hand->setOutcome(Loss);
			}
		}

		// notify the player
		strategy->notifyDealerBlackjack();
	}
}

// checkBlackjack: check each player's hand for Blackjack, pay out 3:2
void Table::checkBlackjack()
{
	Player *player;

	// examine each player
	while((player = _players->iterate()) != NULL)
	{
		Strategy *strategy = player->getStrategy();
		Hand *hand;

		// examine each player's hand
		LinkedList<Hand *> *handList = player->getHands();

		while((hand = handList->iterate()) != NULL)
		{
			// if they have 21, they have a blackjack
			// we set the "blackjack" flag on the hand which will
			// payout 3:2 at payInOut()
			if(hand->getScore() == 21)
			{
				hand->setBlackjack();
				strategy->notifyPlayerBlackjack();
			}
		}
	}
}

// play: allow each player to play until they double, stand or reach a
//       total of 21+
void Table::play()
{
	Player *player;
	PlayAction action = None;

	// allow each player to play
	while((player = _players->iterate()) != NULL)
	{
		Hand *hand;
		Strategy *strategy = player->getStrategy();

		// play each hand
		LinkedList<Hand *> *handList = player->getHands();

		while((hand = handList->iterate()) != NULL)
		{
			bool continuePlaying = true;

			// fetch the cards to pass them to the strategy function
			LinkedList<Card *> *cardList = hand->getCards();

			// they can't play if they have 21
			if(hand->getBlackjack())
				continuePlaying = false;

			// loop until they're done hitting (ie, standing, splitting
			// or doubling all end the players turn
			while(continuePlaying == true)
			{
				// get their action from the strategy
				action = strategy->play(cardList, _dealerShowCards);
				ASSERT(action != None);

				// handle the action - this will add their card on a
				// hit, increase their bet on a double, etc.
				continuePlaying = handleAction(action, player, hand);

				// the player possibly cannot continue on a 21 or bust
				if(continuePlaying && hand->getScore() >= 21)
					continuePlaying = false;
			}
		}
	}

	// the dealer needs to play
	bool continuePlaying = true;

	while(continuePlaying == true)
	{
		// get the dealer's action
		action = _dealerStrategy->play(_dealerCards);
		ASSERT(action != None);

		// handle the action - add a card, etc.
		continuePlaying = handleAction(action, _dealerCards);

		// the dealer cannot possibly continue on a 21 or bust
		if(continuePlaying && Hand::getScore(_dealerCards) >= 21)
			continuePlaying = false;
	}
}

// handleAction: handles the player's action - adds another card to the
//               player's hand if they hit, doubles the bet and adds
//               another card to the player's hand if they double, etc.
//               return true if we can keep playing, false if they're
//               done playing.
bool Table::handleAction(PlayAction action, Player *player, Hand *hand)
{
	LinkedList<Card *> *cards = hand->getCards();
	Hand *splitHand;
	LinkedList<Card *> *splitCards;

	ASSERT(action != None);

	// check the bankroll to make sure double or split can be afforded
	if(action == Double || action == Split)
	{
		// get the bankroll and the bet
		uint16_t bankroll = player->getBankroll();
		uint8_t bet = hand->getBet();

		// the bet is greater than the bankroll, the double or split
		// can't be afforded
		if(bet > bankroll)
		{
			// allow them to buy back in
			Strategy *strategy = player->getStrategy();
			uint16_t buyin = strategy->getBuyin();

			// if they did, give them the chips
			if(buyin > 0)
				player->addBuyin(buyin);

			// otherwise, they don't have enough, so we don't do anything
			// and return true to try to let them play again
			if(bet > player->getBankroll())
				return true;
		}
	}

	switch(action)
	{
		// hit gets one more card and returns true to keep allowing them
		// to take more cards
		case Hit:
			cards->addTail(_shoe->deal());
			return true;

		// double - double bet amount and draw exactly one more card
		case Double:
			// remove more money from their bankroll, put it in their bet
			player->removeBankroll(hand->getBet());
			hand->setBet(hand->getBet() * 2);

			// give them another card
			cards->addTail(_shoe->deal());
			return false;

		// split - turn two paired cards into two different hands
		// splitting aces gives only one card each
		case Split:
			// create a new hand, add it to the hand list
			splitHand = new Hand(true);
			player->addHand(splitHand);

			// remove money from their bankroll, put it in the new hand's
			// bet
			player->removeBankroll(hand->getBet());
			splitHand->setBet(hand->getBet());

			// remove the last card from the original hand, give it
			// to the new "split" hand
			splitCards = splitHand->getCards();
			splitCards->add(cards->removeTail());

			// add a new card to each hand
			cards->addTail(_shoe->deal());
			splitCards->addTail(_shoe->deal());

			// we need to notify the original hand of the split hand
			// for dealing with statistics
			hand->setSplitHand(splitHand);

			// splitting aces only gives one card on each hand
			if(cards->fetchFront()->getRank() == 'A')
				return false;

			// otherwise, they can still play
			else
				return true;

		// stand - leave hand as is
		case Stand:
			return false;

		default:
			ASSERT(0);
	}

	return false;
}

// handleAction: handles the dealer's action - adds another card to the
//               player's hand if they hit - dealer cannot double or
//               split.
bool Table::handleAction(PlayAction action, LinkedList<Card *> *cards)
{
	ASSERT(action != None);

	switch(action)
	{
		// hit gets one more card and returns true to keep allowing them
		// to take more cards
		case Hit:
			cards->addTail(_shoe->deal());
			return true;

		// stand - do nothing
		case Stand:
			return false;

		// dealer can only hit or stand
		default:
			ASSERT(0);
	}

	return false;
}

// payInOut: take players money on a loss, give them money on a win
void Table::payInOut()
{
	Player *player;

	// get the dealer's score
	uint8_t dealerScore = Hand::getScore(_dealerCards);

	// examine each player
	while((player = _players->iterate()) != NULL)
	{
		LinkedList<Hand *> *handList = player->getHands();
		Hand *hand;

		// examine each player's hand
		while((hand = handList->iterate()) != NULL)
		{
			// get the score of this hand and determine their bet
			uint8_t handScore = hand->getScore();
			uint8_t bet = hand->getBet();

			// player has blackjack
			if(hand->getBlackjack())
			{
				// blackjack pays 3:2.  this may mean that we have to add
				// change to their bankroll, but since we only allow integer
				// bets, this would only ever be $0.50 and would only occur
				// when an odd number.  thus, we add the bet * 2.5 (which is
				// always guaranteed by C to be only the *whole number*
				// less than or equal to bet * 2.5, and then if the bet was
				// odd, add $0.50.
				uint8_t bankChange = (bet % 2 == 1) ? 50 : 0;

				player->addBankroll((uint8_t)(bet * 2.5), bankChange);
				hand->setOutcome(Win);
			}

			// bust or dealer has higher cards - dealer wins
			else if
			(
				handScore > 21 ||
				(dealerScore <= 21 && dealerScore > handScore)
			)
			{
				// dealer wins, player does not get payed
				hand->setOutcome(Loss);
			}

			// player has higher cards or dealer bust - player wins
			else if
			(
				handScore <= 21 &&
				(handScore > dealerScore || dealerScore > 21)
			)
			{
				// player wins, gets paid 2:1
				player->addBankroll(bet * 2);
				hand->setOutcome(Win);
			}

			// push - even cards
			else
			{
				// push, move the bet back into his bankroll
				player->addBankroll(bet);
				hand->setOutcome(Push);
			}
		}
	}
}

// writeStats: write to the statistics object of each player the
//             outcome of this particular hand.  we want to keep
//             track of win/loss ratio for this particular type
//             of hand.  (eg, win% for '8 8' against dealer '9'.)
void Table::writeStats()
{
	Player *player;

	// examine each player
	while((player = _players->iterate()) != NULL)
	{
		Hand *hand;
		LinkedList<Hand *> *handList = player->getHands();

		Statistics *statistics = player->getStatistics();

		// examine each player's hand
		while((hand = handList->iterate()) != NULL)
		{
			LinkedList<Card *> *cardList;
			Hand *checkHand;
			HandOutcome overallOutcome;
			bool destroyCardList = false;

			// if this was the hand created from a split, we ignore it
			// because the original hand is put in statistics (below)
			if(hand->isSplitHand())
				continue;

			// examine each split hand to decide if the split was ultimately
			// a win, push or loss -- we have to create a new hand and put
			// the original two cards in it to pass to the stats object
			if(hand->getSplitHand())
			{
				// keep track of the results - if one hand wins and one
				// hand loses, we'll consider that a "push".  (since
				// outcome is no net win or no net loss).  otherwise,
				// two wins = a winning split hand, two losses = a
				// losing split hand
				int8_t results = 0;
				overallOutcome = Push;

				// examine the first cards of the split hands
				for(checkHand = hand; checkHand;
					checkHand = checkHand->getSplitHand())
				{
					HandOutcome splitOutcome = checkHand->getOutcome();

					// add up wins, losses
					if(splitOutcome == Loss)
						results--;
					else if(splitOutcome == Win)
						results++;
				}

				// determine overall outcome
				if(results > 0)
					overallOutcome = Win;
				else
					overallOutcome = Loss;

				// we need to make a new cardList that contains the
				// two cards which were split
				cardList = new LinkedList<Card *>();
				cardList->addTail(hand->getCards()->fetchFront());
				cardList->addTail(hand->getSplitHand()->getCards()->fetchFront());

				// we malloced this card list, we need to destroy it
				// below
				destroyCardList = true;
			}
			else
			{
				// otherwise, there was no split, so we just pass the
				// hand and the outcome to the stats object
				overallOutcome = hand->getOutcome();
				cardList = hand->getCards();
			}

			// notify the stats object of what happened
			statistics->write(_dealerShowCards, cardList, overallOutcome);

			// destroy the card list if it was created from a split
			if(destroyCardList)
				delete cardList;

			// dump the statistics to the console every 25,000 hands
			if(++_handCount % 250000 == 0)
				statistics->dump(player);
		}
	}
}

// summarize: tell the player the outcome of this hand
void Table::summarize()
{
	Player *player;

	// tell each player
	while((player = _players->iterate()) != NULL)
	{
		LinkedList<Hand *> *handList = player->getHands();

		Strategy *strategy = player->getStrategy();

		// summarize each hand for the player
		strategy->summarize(_dealerCards, handList);
	}
}

Table::~Table()
{
	delete _shoe;

	delete _dealerStrategy;
	delete _dealerCards;
	delete _dealerShowCards;
}

