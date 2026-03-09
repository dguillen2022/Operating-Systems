#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_ARGS 256

/* Structure used to store information about each process. */
struct Process_Data {
	pid_t pid;
	time_t init_time;
	int num_order;
};

typedef struct Process_Data Process_Data;

int
main(int argc, char *argv[])
{
	pid_t pid, cpid;
	int wstatus, i, j, general_status = EXIT_SUCCESS;
	char *saveptr, *status, *tok, *tmp;
	Process_Data data;
	Process_Data *process_list = malloc(sizeof(Process_Data) * (argc - 1));

	if (process_list == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	char **execv_args = malloc(sizeof(char *) * MAX_ARGS);

	if (execv_args == NULL) {
		perror("malloc");
		free(process_list);
		exit(EXIT_FAILURE);
	}

	/* If less than 2 arguments are given, the program ends with error. */
	if (argc < 2) {
		fprintf(stderr, "usage: timecmds 'program arguments'...\n");
		exit(EXIT_FAILURE);
	}

	/* For each program given as an argument, a process is created */
	for (i = 1; i < argc; i++) {
		cpid = fork();

		switch (cpid) {
		case -1:
			perror("fork");
			free(process_list);
			free(execv_args);
			exit(EXIT_FAILURE);

		case 0:
			/* Split the argument corresponding to the process for execv */
			/* A temporal variable is used for strtok not to modify the original string argv. */
			tmp = strdup(argv[i]);
			if (tmp == NULL) {
				perror("strdup");
				free(process_list);
				free(execv_args);
				exit(EXIT_FAILURE);
			}

			execv_args[0] = strtok_r(tmp, " ", &saveptr);

			for (j = 1; j < MAX_ARGS; j++) {
				tok = strtok_r(NULL, " ", &saveptr);

				if (tok == NULL) {
					execv_args[j] = NULL;
					break;
				}
				execv_args[j] = tok;
			}

			/* If the argument's list is full and doesn't end with NULL exits with error */
			if (j == MAX_ARGS && tok != NULL) {
				fprintf(stderr, "Error: Token limit exceeded");
				free(process_list);
				free(execv_args);
				free(tmp);
				exit(EXIT_FAILURE);
			}

			execv(execv_args[0], execv_args);
			perror("execv");
			free(process_list);
			free(execv_args);
			free(tmp);	/* strdup obtains memory using malloc and it can be freed */
			exit(EXIT_FAILURE);

		default:
			/* The parent process stores the child data for later printing. */
			data.pid = cpid;
			data.init_time = time(NULL);
			data.num_order = i;
			process_list[i - 1] = data;
			break;
		}
	}

	while ((pid = wait(&wstatus)) != -1) {

		for (i = 0; i < argc - 1; i++) {

			/* If the pid matches, the process's string is printed */
			if (process_list[i].pid == pid) {

				if (WIFEXITED(wstatus)
				    && WEXITSTATUS(wstatus) == 0) {
					status = "success";
				} else {
					status = "failure";
					general_status = EXIT_FAILURE;
					/* If any program fails, the parent exits with failure */
				}

				printf
				    ("cmd: %s, pid: %d, time: %ld seconds, status: %s\n",
				     argv[i + 1], pid,
				     (long)(time(NULL) -
					    process_list[i].init_time), status);
			}
		}
	}

	free(process_list);
	free(execv_args);
	exit(general_status);
}
