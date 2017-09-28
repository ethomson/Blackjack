// Blackjack Simulation : StrategyInteractive
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
// This isn't a "strategy" per se, but it still controls the playing
// from stdio.  Note: C++'s iostreams are so horrible for printing
// formatted text that we use printf.  Yes, it's possible to go too far
// with objects.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "StrategyInteractive.h"
#include "Strategy.h"
#include "Card.h"
#include "Hand.h"

StrategyInteractive::StrategyInteractive()
{
	_bet = 1;
}

// getBuyin: prompts the user and returns amount to buyin at table
//           sit-down and when out of funds
uint16_t StrategyInteractive::getBuyin()
{
	uint16_t buyin = 0;

	while(buyin == 0)
		buyin = getInput("Enter buyin amount", 100);

	printf("\n");

	return buyin;
}

// getBet: prompts the user and returns the amount to bet on this hand
uint8_t StrategyInteractive::getBet(uint16_t bank, uint16_t cardsLeft)
{
	uint8_t bet = 0;

	// suppress unused warnings
	cardsLeft = cardsLeft;

	printf("You have $%d.  ", bank);

	while(bet == 0)
		bet = getInput("Enter bet amount", _bet);

	_bet = bet;

	return bet;
}

// play: player for the dealer - interactive strategy can't be used for
//       the dealer
PlayAction StrategyInteractive::play(LinkedList<Card *> *cardList)
{
	// suppress unused variable warnings
	cardList = cardList;

	// interactive strategy cannot be used for dealer
	ASSERT(0);

	return None;
}

// play: prompts the player and returns way to play
PlayAction StrategyInteractive::play(LinkedList<Card *> *cardList,
 LinkedList<Card *> *dealerCards)
{
	Card *card;
	char input = 0;
	char prompt[128];

	// get the score for this hand
	uint8_t handScore = Hand::getScore(cardList);

	// longest hand is used to determine how to pad the display
	uint8_t longestHand = (cardList->getCount() > 2) ? (uint8_t)cardList->getCount() : 2;


	printf("\nYour turn to play:\n");


	// display the dealer's cards (* * for the first card indicates
	// dealer's down card)
	printf("DEALER:  **  ");

	// iterate through dealer's other cards, displaying them
	while((card = dealerCards->iterate()) != NULL)
		printf("%c%c  ", card->getRank(), card->getSuit());

	// pad the display
	if(dealerCards->getCount() < longestHand)
	{
		for(int i = dealerCards->getCount(); i < longestHand; i++)
			printf("    ");
	}
	printf("\n");


	// display the player's cards, iterate through each card, displaying
	// them
	printf("PLAYER:  ");
	while((card = cardList->iterate()) != NULL)
	{
		printf("%c%c  ", card->getRank(), card->getSuit());
	}


	// print the player's score
	printf("[");

	// print soft score, if available
	if(Hand::getSoft(cardList))
		printf("%d/", (handScore - 10));

	printf("%d]\n\n", handScore);


	// loop until valid play input provided
	while(input != 'h' && input != 's' && input != 'd' && input != 'p')
	{
		bool doubleAllowed = cardList->getCount() == 2;
		bool splitAllowed = Hand::splitAllowed(cardList);

		// build the prompt for input based on available play
		strcpy_s(prompt, 128, "Your action:  S)tand, H)it");

		if(doubleAllowed)
			strcat_s(prompt, 128, ", D)ouble");

		if(splitAllowed)
			strcat_s(prompt, 128, ", s(P)lit");

		input = getInput(prompt, (char)0);

		// double not allowed
		if(input == 'd' && doubleAllowed == false)
		{
			printf("You cannot double this hand.\n");
			input = 0;
		}

		// split not allowed
		if(input == 'p' && splitAllowed == false)
		{
			printf("You cannot split this hand.\n");
			input = 0;
		}
	}

	if(input == (int)'h')
		return Hit;
	else if(input == (int)'s')
		return Stand;
	else if(input == (int)'d')
		return Double;
	else if(input == (int)'p')
		return Split;

	ASSERT(0);
	return None;
}

// insure: prompts and returns true if dealer wishes to insure their hand
bool StrategyInteractive::insure(LinkedList<Card *> *dealerCards,
 LinkedList<Card *> *cardList)
{
	Card *card;
	char input = 0;

	printf("\nINSURANCE OFFER:\n");

	// display the dealer cards
	printf("DEALER:  * *  ");

	// iterate through dealer's other cards, displaying them
	while((card = dealerCards->iterate()) != NULL)
		printf("%c%c  ", card->getRank(), card->getSuit());

	printf("\n");


	// display the player's cards, iterating through each one
	printf("PLAYER:  ");
	while((card = cardList->iterate()) != NULL)
		printf("%c%c  ", card->getRank(), card->getSuit());

	// display the score
	printf("[%02d]    ", Hand::getScore(cardList));

	printf("\n\n");


	// offer insurance
	const char *prompt = (Hand::getScore(cardList) == 21) ?
	 "Insure this hand -- take even money?" : "Insure this hand?";

	// loop until they give us a valid answer
	while(input != 'y' && input != 'n')
	{
		input = getInput(prompt, 'n');
	}

	return ((input == 'y') ? true : false);
}

// notifyShuffle: displays message indicating dealer is shuffling
void StrategyInteractive::notifyShuffle(void)
{
	printf("\nNOTICE: Dealer is Shuffling...\n");
}

// notifyDealerBlackjack: displays message indicating that dealer has
//                        BlackJack
void StrategyInteractive::notifyDealerBlackjack(void)
{
	printf("\nNOTICE: Dealer has BlackJack.\n");
}

// notifyPlayerBlackjack: displays message indicating that player has
//                        BlackJack
void StrategyInteractive::notifyPlayerBlackjack(void)
{
	printf("\nNOTICE: You have BlackJack.\n");
}

// summarize: displays a summary showing the end of the hand
void StrategyInteractive::summarize(LinkedList<Card *> *dealerCards,
 LinkedList<Hand *> *hands)
{
	uint8_t score;
	Card *card;
	Hand *hand;
	uint8_t handCount = 0;

	// find the greatest number of cards in each hand for padding
	uint8_t longestHand = (uint8_t)dealerCards->getCount();

	while((hand = hands->iterate()) != NULL)
	{
		uint8_t cardCount = (uint8_t)hand->getCards()->getCount();

		// this hand is longer
		if(cardCount > longestHand)
			longestHand = cardCount;
	}


	printf("\nHand Summary:\n");


	// display the dealer cards, iterate through each one
	printf("DEALER:  ");
	while((card = dealerCards->iterate()) != NULL)
		printf("%c%c  ", card->getRank(), card->getSuit());

	// add some padding so that totals line up in each column
	if(dealerCards->getCount() < longestHand)
	{
		for(int i = dealerCards->getCount(); i < longestHand; i++)
			printf("    ");
	}

	// display score (or bust)
	if((score = Hand::getScore(dealerCards)) > 21)
		printf("[BUST]  ");
	else
		printf("[%02d]    ", score);

	printf("\n");


	// dump each player hand
	while((hand = hands->iterate()) != NULL)
	{
		HandOutcome outcome;
		LinkedList<Card *> *cardList = hand->getCards();

		if(handCount++ == 0)
			printf("PLAYER:  ");
		else
			printf("         ");

		// display each card in this hand
		while((card = cardList->iterate()) != NULL)
			printf("%c%c  ", card->getRank(), card->getSuit());

		// add some padding, if necessary, so that hand total lines up
		// in each column
		if(cardList->getCount() < longestHand)
		{
			for(int i = cardList->getCount(); i < longestHand; i++)
				printf("    ");
		}

		// display score (or bust) for this hand
		if((score = hand->getScore()) > 21)
			printf("[BUST]  ");
		else
			printf("[%02d]    ", score);

		// display the outcome for this hand (Win, Push, Loss)
		if((outcome = hand->getOutcome()) == Win)
			printf("--  WIN");
		else if(outcome == Push)
			printf("--  PUSH");
		else
			printf("--  LOSS");

		printf("\n");
	}

	printf("\n");
}

// getInput: displays a prompt and returns the input collected from stdin
//           this expects character input (or string input, and will return
//           the first character).  all input is lower-cased
char StrategyInteractive::getInput(const char *prompt, char defaultInput)
{
	char input[256] = "\0";

	while(input[0] == '\0')
	{
		// display the prompt (if there's a default input, display it
		// so they know what the input is if they just hit "enter")
		if(defaultInput > 0)
			printf("%s [%c]: ", prompt, defaultInput);
		else
			printf("%s: ", prompt);

		// get some input from stdin
		fgets(input, 256, stdin);

		// if they entered a super-long string, we want to burn through the
		// input up to the newline to avoid keeping crap in our input buffer
		if(strchr(input, '\n') == NULL)
		{
			while(strchr(input, '\n') == NULL)
				fgets(input, 256, stdin);
			input[0] = '\0';
		}

		// the user just hit enter, but there's no default value, so
		// we need to prompt them again
		if(input[0] == '\n' && defaultInput == 0) 
			input[0] = '\0';
	}

	// user hit enter, simply return the default input
	if(input[0] == '\n')
		return defaultInput;

	// otherwise, return the (lowercased) first character of their input
	else
		return tolower(input[0]);
}

// getInput: displays a prompt and returns the input collected from stdin
//           this expects numeric input.
uint8_t StrategyInteractive::getInput(const char *prompt, int defaultInput)
{
	char input[256] = "\0";
	int converted = 0;

	while(input[0] == '\0')
	{
		// display the prompt (if there's a default input, display it
		// so they know what the input is if they just hit "enter")
		if(defaultInput >= 0)
			printf("%s [%d]: ", prompt, defaultInput);
		else
			printf("%s: ", prompt);

		// get some input from stdin
		fgets(input, 256, stdin);

		// if they entered a super-long string, we want to burn through the
		// input up to the newline to avoid keeping crap in our input buffer
		if(strchr(input, '\n') == NULL)
		{
			while(strchr(input, '\n') == NULL)
				fgets(input, 256, stdin);
			input[0] = '\0';
		}

		// (try) to convert their string input into a decimal
		converted = atoi(input);

		// the user just hit enter, but there's no default value, so
		// we need to prompt them again
		if(input[0] == '\n' && defaultInput < 0) 
			input[0] = '\0';

		// the user did not enter a valid number
		if(input[0] != '\n' && (converted < 0 || converted > UINT8_MAX))
			input[0] = '\0';
	}

	// user hit enter, simply return the default input
	if(input[0] == '\n')
		return (uint8_t)defaultInput;

	// otherwise, return their input
	else
		return converted;
}

