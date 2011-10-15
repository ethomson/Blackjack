// Blackjack Simulation : StrategyLookup
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
// This contains functions for easy lookup of Strategy types by name.

#ifndef STRATEGYLOOKUP_H
#define STRATEGYLOOKUP_H

#include <stdlib.h>
#include <sys/types.h>
#include "Util.h"
#include "Strategy.h"


// the strategy list structure - we don't need a class and it would
// be harder to define (see StrategyLookup.cpp to see why.)
struct StrategyList {
	char *name;
	char *description;
};


// return a strategy object by name (eg, "dealer" returns a
// StrategyVegasDealer object
Strategy *StrategyLookup(char *name);
void StrategyList(FILE *fptr = stdout);

#endif // STRATEGYLOOKUP_H
