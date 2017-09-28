// Blackjack Simulation
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"
#include "Table.h"
#include "StrategyLookup.h"

int main(int argc, char **argv)
{
	Table *table;

	// default player type is interactive player (playing on console)
	const char *playerType = "interactive";

	// if they provide a strategy type on the command line, let's
	// use it
	if(argc > 1)
		playerType = argv[1];

	// try to instantiate a table
	try {
		table = new Table();
	}
	catch(...) {
		fprintf(stderr, "Could not instantiate table object.\n");
		exit(1);
	}

	// add the requested player type
	try {
		table->addPlayer(playerType);
	}
	catch(...) {
		// could instantiate that type of player strategy.  let's
		// display a list
		fprintf(stderr, "Could not create %s player.  ", playerType);
		fprintf(stderr, "Valid player types are:\n\n");

		StrategyList(stderr);

		fprintf(stderr, "\n");

		return 1;
	}

	// run the table loop
	table->loop();

	// teardown the table object
	delete table;

	return 0;
}
