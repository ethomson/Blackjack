// Blackjack Simulation : StrategyLookup
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
// This contains functions for easy lookup of Strategy types by name.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <stdexcept>
#include <sys/types.h>
#include "Util.h"
#include "StrategyLookup.h"
#include "Strategy.h"

// to add new Strategy types to this list, first include their appropriate
// header file
#include "StrategyBasic.h"
#include "StrategyCardCount.h"
#include "StrategyInteractive.h"
#include "StrategyDealerVegas.h"
#include "StrategyDealerHitsSoft17.h"

// then add them to this list (with a unique name), making sure that NULL
// stays at the end of the list
struct StrategyList strategyList[] = {
	{ "basic", "Basic Blackjack Strategy" },
	{ "cardcount", "Simple 10s Count Strategy" },
	{ "interactive", "Interactive -- User is prompted for play" },
	{ "dealer", "Dealer Strategy (Stands on soft 17)" },
	{ "dealerhitssoft17", "Dealer Strategy (Hits on soft 17)" },
	{ NULL, NULL },
};

Strategy *StrategyLookup(const char *name)
{
	Strategy *obj = NULL;

	// finally, add a name comparison and instantiation here
	if(strcasecmp(name, "basic") == 0)
	{
		obj = new StrategyBasic();
	}
	else if(strcasecmp(name, "cardcount") == 0)
	{
		obj = new StrategyCardCount();
	}
	else if(strcasecmp(name, "dealer") == 0)
	{
		obj = new StrategyDealerVegas();
	}
	else if (strcasecmp(name, "dealerhitssoft17") == 0)
	{
		obj = new StrategyDealerHitsSoft17();
	}
	else if(strcasecmp(name, "interactive") == 0)
	{
		obj = new StrategyInteractive();
	}

	if(obj == NULL)
	{
		throw std::invalid_argument("unknown strategy name");
	}

	return obj;
}

void StrategyList(FILE *fptr)
{
	for(int i = 0; strategyList[i].name != NULL; i++)
	{
		fprintf(fptr, " %12s - %s\n", strategyList[i].name,
		 strategyList[i].description);
	}
}

