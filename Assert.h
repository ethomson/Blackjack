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
// message when sanity checks fail.  Calling ASSERT() with a true
// statement will cause the program to continue, calling ASSERT()
// with a false statement will cause the program to abort(3).
// (These only occur when compiled with -DDEBUG.)
//
// ASSERT() is a macro so that we don't need to bother with a
// function call in release mode.
//
// NOTE: this will probably not work under VisualStudio7 because of its
// weak macro syntax.  It does work under gcc and VisualStudio.NET.

#ifndef ASSERT_H
#define ASSERT_H

#ifdef DEBUG
# ifdef VC6 // VC6 doesn't have __FUNCTION macro
#  define __FUNCTION__	"(unknown)"
# endif

# define ASSERT(exp)		(void)((exp || \
	(AssertFailure(#exp, __FILE__, __FUNCTION__, __LINE__), 0)))

void AssertFailure(const char *expr, const char *file, const char *func,
	int line);


#else // DEBUG
# define ASSERT(exp)	((void) 0)
#endif // DEBUG

#endif // ASSERT_H

