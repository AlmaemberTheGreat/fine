/* See COPYING for licence and copyright information */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "con.h"

static bool parsenum(char *str, unsigned long *out);

static void execute(char *args[], size_t len);
static void runprg(char *args[], size_t len);

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

	if (len < 1) {
		fprintf(stderr, "a command is required\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < nwarmup; ++i) {
		prstat(WARMUP, nwarmup, i + 1);
		runprg(args, len);
	}

	for (i = 0; i < nrun; ++i) {
		prstat(TESTING, nrun, i + 1);
		runprg(args, len);
	}
}

static void
runprg(char *args[], size_t len)
{
	pid_t id;

	id = fork();
	if (id < 0) {
		/* failed */
		perror("unable to fork");
		exit(EXIT_FAILURE);
	} else if (id > 0) {
		/* parent process */
	} else {
		/* child process */
		int fd;

		/* we don't care about the output, so let's "mute" it */
		fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		if (fd < 0) {
			perror("unable to open /dev/null");
			exit(EXIT_FAILURE);
		}

		/*dup2(fd, 1);
		dup2(fd, 2);*/

		execvp(args[0], args);
		/* if we're still here, something bad happened */
		perror("failed to exec");
		exit(EXIT_FAILURE);
	}
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
