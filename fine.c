/* See COPYING for licence and copyright information */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void execute(char *args[], size_t len);

static unsigned long nwarmup = 0;

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
			unsigned long num;
			char *numend;

			num = strtoul(optarg, &numend, 10);
			if (*optarg == '\0' || *numend != '\0') {
				fprintf(stderr, "%s: error -- not a valid number: '%s'\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}

			nwarmup = num;
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
