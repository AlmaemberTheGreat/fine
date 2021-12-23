/* See COPYING for licence and copyright information */

#include <stdio.h>

#include "con.h"

#define ESC "\x1B"

void
prstat(enum Phase phase, unsigned long max, unsigned long val)
{
	char *msg;

	if (phase == WARMUP)
		msg = "Warming up...";
	else
		msg = "Testing...";

	/* Go to beginning of line */
	fputs(ESC "[1F" ESC "[K", stderr);
	fflush(stderr);

	fprintf(stderr, "%s (%lu/%lu)\n", msg, val, max);
}
