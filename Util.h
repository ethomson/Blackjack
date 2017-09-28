// Blackjack Simulation : Util
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
// This defines miscellaneous functions, macros and types that are
// missing on some systems for compatibility.

#ifndef UTIL_H
#define UTIL_H

// Very old MSVC doesn't have stdint.h, we need to defint integral
// types and their limits.
#ifdef NEEDS_INT_TYPES
#define INT8_MAX		(127)
#define INT16_MAX		(32767)
#define INT32_MAX		(2147483647)
#define UINT8_MAX		(255)
#define UINT16_MAX		(65535)
#define UINT32_MAX		(4294967295U)
#endif // NEEDS_INT_TYPES


// Fill-in functions Microsoft lacks
#ifdef WIN32
# include <windows.h>

# define strcasecmp			_stricmp		// strcasecmp(a, b)
#else // WIN32
# include <unistd.h>
#endif // WIN32


// Seed the random number generator in a cross-platform manner
#ifdef WIN32
# define srand_portable()	srand((unsigned int)time(NULL) ^ (unsigned int)GetCurrentThreadId() ^ (unsigned int)GetCurrentProcessId())
#else // WIN32
# define srand_portable()	srand(time(NULL) ^ (getpid() + (getpid() << 15)))
#endif // WIN32


// Clear the screen in a cross-platform manner
#ifdef WIN32
# define clear_portable()	system("cls")
#else // WIN32
# define clear_portable()	system("clear")
#endif // WIN32


// Microsoft doesn't have stdint.h, so we'll define these here
#ifdef NEEDS_INT_TYPES
typedef signed char   int8_t;
typedef short int     int16_t;
typedef int           int32_t;
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned long uint32_t;
#endif // NEEDS_INT_TYPES

#endif // UTIL_H
