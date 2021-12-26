/* See COPYING for licence and copyright information */

#include <stdio.h>

#include "cons.h"

void
prstat(enum Phase phase, unsigned long max, unsigned long val)
{
	char *msg;

	if (phase == WARMUP)
		msg = "Warming up...";
	else
		msg = "Testing...";

	/* Go to beginning of line */
	fputs("\x1b[1F\x1b[K", stderr);
	fflush(stderr);

	fprintf(stderr, "%s (%lu/%lu)\n", msg, val, max);
}
