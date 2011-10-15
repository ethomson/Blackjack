// Blackjack Simulation : Utility Library : Assert
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
// Assertion routines - halts program execution with a helpful error
// message when sanity checks fail.  Calling assert() with a true
// statement will cause the program to continue, calling assert()
// with a false statement will cause the program to abort(3).
//
// NOTE: this will probably not work under VisualStudio7 because of its
// weak macro syntax.  It does work under gcc and VisualStudio.NET.

#include <stdio.h>
#include <stdlib.h>
#include "Assert.h"
#include "Util.h"

// AssertFailure: called by the ASSERT macro (see Assert.h) when an
//                assertion fails
void AssertFailure(const char *expr, const char *file, const char *func,
	int line)
{
	fprintf(stderr, "*** ASSERTION FAILED ***\n");
	fprintf(stderr, ">> %s << in %s:%s() line %d\n", expr, file, func, line);
	fprintf(stderr, "*** ASSERTION FAILED ***\n");

	abort();
}

