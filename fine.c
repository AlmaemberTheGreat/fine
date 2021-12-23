/* See COPYING for licence and copyright information */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "con.h"

static bool parsenum(char *str, unsigned long *out);

static void execute(char *args[], size_t len);

static unsigned long nwarmup = 0,
                     nrun    = 0;
static char         *prepcmd = NULL;
static bool          quiet   = false;

static bool
parsenum(char *str, unsigned long *out)
{
	unsigned long num;
	char *numend;

	num = strtoul(optarg, &numend, 10);
	if (*optarg == '\0' || *numend != '\0') {
		return false;
	}

	*out = num;
	return true;
}

static void
execute(char *args[], size_t len)
{
	puts("");
}

int
main(int argc, char *argv[])
{
	char opt;

	while ((opt = getopt(argc, argv, ":w:")) != -1) {
		switch (opt) {
		case 'w': {
			if (!parsenum(optarg, &nwarmup)) {
				fprintf(stderr, "%s: error -- not a valid number: '%s'\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}

		case 'r': {
			if (!parsenum(optarg, &nrun)) {
				fprintf(stderr, "%s: error -- not a valid number: '%s'\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}

		case 'p': {
			prepcmd = optarg;
			break;
		}

		case 'q': {
			quiet = true;
			break;
		}

		case ':':
			fprintf(stderr, "%s: error -- option '%c' requires an argument\n", argv[0], optopt);
			exit(EXIT_FAILURE);
			break;

		case '?':
			fprintf(stderr, "%s: error -- invalid option '%c'\n", argv[0], optopt);
			exit(EXIT_FAILURE);
			break;
		}
	}

	execute(argv + optind, argc - optind);
}
