/* See COPYING for licence and copyright information */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "con.h"

static bool parsenum(char *str, unsigned long *out);

static void execute(char *args[], size_t len);
static int  runprg(char *args[], size_t len, unsigned long long *delta);

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
	size_t i;
	unsigned long long delta,
	                   whole = 0,
	                   avg,
	                   min = 0,
	                   max = 0;
	double             maxsec,
	                   minsec,
	                   avgsec;

	putchar('\n');

	if (len < 1) {
		fprintf(stderr, "a command is required\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < nwarmup; ++i) {
		if (!quiet) prstat(WARMUP, nwarmup, i + 1);
		runprg(args, len, NULL);
	}

	for (i = 0; i < nrun; ++i) {
		if (!quiet) prstat(TESTING, nrun, i + 1);
		runprg(args, len, &delta);
		whole += delta;
		if (delta < min || min == 0) min = delta;
		if (delta > max) max = delta;
	}

	avg = whole / nrun;

	maxsec = (double)max / 1000000;
	minsec = (double)min / 1000000;
	avgsec = (double)avg / 1000000;
	fprintf(stderr, "minimum: %.3fs\nmaximum: %.3fs\naverage: %.3fs\n", minsec, maxsec, avgsec);
}

static int
runprg(char *args[], size_t len, unsigned long long *delta)
{
	pid_t id;
	int status;
	struct timespec beg, end;

	id = fork();
	if (id < 0) {
		/* failed */
		perror("unable to fork");
		exit(EXIT_FAILURE);
	} else if (id > 0) {
		/* parent process */
		clock_gettime(CLOCK_MONOTONIC_RAW, &beg);
		wait(&status);
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		if (delta != NULL) *delta = (end.tv_sec - beg.tv_sec) * 1000000 + (end.tv_nsec - beg.tv_nsec) / 1000;
	} else {
		/* child process */
		int fd;

		/* we don't care about the output, so let's "mute" it */
		fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		if (fd < 0) {
			perror("unable to open /dev/null");
			exit(EXIT_FAILURE);
		}

		dup2(fd, 1);
		dup2(fd, 2);

		execvp(args[0], args);
		/* if we're still here, something bad happened */
		perror("failed to exec");
		exit(EXIT_FAILURE);
	}

	return status;
}

int
main(int argc, char *argv[])
{
	char opt;

	while ((opt = getopt(argc, argv, ":w:r:p:q")) != -1) {
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
