// Blackjack Simulation : Statistics
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
// This is the statistics class, which keeps track of wins/losses
// for each (initially dealt) hand combination.  This is useful for
// tracking how well your strategy is working, whether you create a
// new strategy or are playing interactively to test your memory of
// a strategy.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Assert.h"
#include "LinkedList.h"
#include "Card.h"
#include "Statistics.h"
#include "Hand.h"
#include "Player.h"

Statistics::Statistics()
{
	// NOTE: we initialize these arrays with some padding -- we access
	// each card with its rank - ie, a 2 is at index 2 - so there's no
	// card at index 0.  So, we could either subtract 1 from the card
	// and confuse ourselves (ie, card 2 is at index 1) or waste a few
	// bytes.  Code cleanliness is worth a few bytes, since nobody
	// really wants an embedded version of this (I hope.)

	// we keep the statistics array so that we can lookup the player's
	// hand by either pair, ace + kicker or total sum of cards and
	// dealer's showing card.  Examples:
	// dealer=6 player=8/8 win =  _stats[6][8][8][Win]
	// dealer=9 player=A/5 loss = _stats[9][1][5][Loss]
	// dealer=A player=9/8 push = _stats[11][17][0][Push]

	// loop through each card that the dealer could have showing
	for(int i = 0; i < 12; i++)
	{
		// loop through each single card and total that could be had
		// by the player.
		for(int j = 0; j < 22; j++)
		{
			// loop through each second card that could be had by the
			// player.
			for(int k = 0; k < 12; k++)
			{
				// loop through each possible outcome (Win, Push, Loss)
				for(int l = 0; l < 3; l++)

					// and zero out the stat count for that
					_stats[i][j][k][l] = 0;
			}
		}
	}
}

// write: increment the outcome count for this dealer show card and player
//        initial deal.  ie, if the dealer had a 7 showing, the player's
//        initial cards were 9 8 and the player won, we would do:
//        _stats[7][17][0][Push]++;
void Statistics::write(LinkedList<Card *> *dealerCards,
 LinkedList<Card *> *playerCards, HandOutcome result)
{
	uint8_t dealerScore, playerScore1, playerScore2;

	// get the player's first two cards (initial deal)
	Card *playerCard1 = playerCards->fetchFront();
	Card *playerCard2 = playerCards->fetchNumber(2);

	// get the rank of their cards
	char playerRank1 = playerCard1->getRank();
	char playerRank2 = playerCard2->getRank();

	// determine how to arrange the dealer score
	dealerScore = getScore(dealerCards->fetchFront());

	// find out if we have matching cards - they get their statistics
	// computed differently, since most strategies have different play
	// for paired cards (ie, we may sometimes split)
	if(getScore(playerCard1) == getScore(playerCard2))
	{
		playerScore1 = getScore(playerCard1);
		playerScore2 = playerScore1;
	}

	// find out if we have a single ace - they also get their statistics
	// computed differently, since most strategies have different play
	// for soft hands (ie, we may sometimes double)
	else if(playerRank1 == 'A' || playerRank2 == 'A')
	{
		Card *otherCard = (playerRank1 == 'A') ? playerCard2 : playerCard1;

		playerScore1 = 1;
		playerScore2 = getScore(otherCard);
	}

	// otherwise, add up the score, since it doesn't matter whether you
	// have 10 + 7 or 9 + 8 when deciding how to play
	else
	{
		playerScore1 = getScore(playerCard1) + getScore(playerCard2);
		playerScore2 = 0;
	}

	// increment the count for this result
	_stats[dealerScore][playerScore1][playerScore2][result]++;
}

// dump: write the statistics to the console (or a file)
void Statistics::dump(Player *player, FILE *fptr)
{
	// clear the screen if we're writing to the console
	if(fptr == stdout)
		clear_portable();

	// this is the dealer's hand
	fprintf(fptr, "       A      2      3      4      5      6      7      8      9     10\n");

	// write hands without an ace or a split
	for(int plyr = 5; plyr < 20; plyr++)
		dumpHand(plyr, 0);

	// write hands with an ace
	for(int plyr = 2; plyr < 11; plyr++)
		dumpHand(1, plyr);

	// write hands with paired cards
	for(int plyr = 1; plyr < 11; plyr++)
		dumpHand(plyr, plyr);


	// number of hands played
	uint32_t handsPlayed = player->handsPlayed();

	// determine the winningness/losingness of this strategy
	uint32_t buyin = player->getBuyin();
	uint16_t bankroll = player->getBankroll();

	// amount won
	int16_t winAmount = (int16_t)(bankroll - buyin);

	// income (payin/payout) per hand
	double incomePerHand = ((double)winAmount / (double)handsPlayed);

	fprintf(fptr,
	 "             HANDS PLAYED: %d,  INCOME/HAND: $%f\n",
	 handsPlayed, incomePerHand);

	fprintf(fptr,
	 "      AMOUNT BOUGHT IN: $%d,  CURRENT BANKROLL: $%d,  WINS: $%d\n",
	 buyin, bankroll, winAmount);
}

// dumpHand: write the statistics for a player's hand to the console
//           (or a file)
void Statistics::dumpHand(uint8_t card1, uint8_t card2, FILE *fptr)
{
	// pretty-print the aces
	if(card1 == 1 && card2 == 1)
		fprintf(fptr, " A/A  ");

	// print an ace with a kicker
	else if(card1 == 1 && card2 != 0)
		fprintf(fptr, " A/%-2d ", card2);

	// otherwise, print the total
	else if(card2 == 0)
		fprintf(fptr, "  %2d  ", card1);

	// print a paired card
	else
		fprintf(fptr, "%2d/%-2d ", card1, card2);

	// loop through each dealer card
	for(int dlr = 1; dlr < 11; dlr++)
	{
		// total number of hands played is wins + losses + pushes
		int total = _stats[dlr][card1][card2][Win] +
		 _stats[dlr][card1][card2][Loss] +
		 _stats[dlr][card1][card2][Push];

		// we'll consider wins or pushes as "successful" hands since
		// we didn't lose money
		int win = _stats[dlr][card1][card2][Win] +
		 _stats[dlr][card1][card2][Push];


		// no hands were played with these two cards
		if(total == 0)
			fprintf(fptr, "NONE   ");

		// all hands have won - to fit in 80 cols, we'll just print
		// 99.9%.
		else if(win == total)
			fprintf(fptr, "99.9%%  ");

		// otherwise, compute the successfulness of the hand
		else
		{
			// we use a float here, and cutoff at 1 decimal place
			float winPercent = (((float)win / (float)total) * 100);

			fprintf(fptr, "%4.1f%%  ", winPercent);
		}
	}

	fprintf(fptr, "\n");
}

// getScore: gets the score suitable for putting in the array
//           (in particular, A = 1)
uint8_t Statistics::getScore(Card *card)
{
	uint8_t score = 0;

	char rank = card->getRank();

	// A = 1 instead of 11, to put in an array
	if(rank == 'A')
		score = 1;

	// 10 cards
	else if(rank == 'T' || rank == 'J' || rank == 'Q' || rank == 'K')
		score = 10;

	// otherwise, we have a char like '6', so subtracting 48 gives us
	// the decimal value (ie, '6' = ASCII 54, subtracting 48 gives us 6).
	else
		score = rank - 48;

	return score;
}

Statistics::~Statistics()
{
}

