#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARGS_PER_WORD 2
#define PRE_WORDS_ARGS 2
#define POST_WORDS_ARGS 2

int
main(int argc, char *argv[])
{
	int i, word_count, max_args;
	pid_t pid;
	char **args;

	/* If there are less than 2 arguments given it exits with failure */
	if (argc < 3) {
		fprintf(stderr, "usage: myfgrep word [word]... file\n");
		exit(EXIT_FAILURE);
	}

	word_count = argc - 2;

	/* Max arguments used when calling execv */
	max_args =
	    ARGS_PER_WORD * word_count + PRE_WORDS_ARGS + POST_WORDS_ARGS;

	args = malloc(sizeof(char *) * max_args);
	if (args == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	switch (pid) {
	case -1:
		perror("fork");
		free(args);
		exit(EXIT_FAILURE);

	case 0:
		/* Args at the beginning */
		args[0] = "/bin/fgrep";
		args[1] = "-n";

		/* Each word with the argument "-e" */
		for (i = 1; i < argc - 1; i++) {
			args[ARGS_PER_WORD * i] = "-e";
			args[ARGS_PER_WORD * i + 1] = argv[i];
		}

		/* Args at the end */
		args[max_args - 2] = argv[argc - 1];
		args[max_args - 1] = NULL;

		execv("/bin/fgrep", args);
		perror("execv");
		free(args);
		exit(EXIT_FAILURE);

	default:
		free(args);
		exit(EXIT_SUCCESS);
	}
}
