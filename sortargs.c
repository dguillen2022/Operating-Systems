#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UPPERCASE_CONST 32

void
insertion_sort(char *arr[], int n)
{
	int i, j;
	char *key;

	for (i = 1; i < n; i++) {
		key = arr[i];
		j = i - 1;

		while (j >= 0 && strcmp(arr[j], key) > 0) {
			arr[j + 1] = arr[j];
			j--;
		}

		arr[j + 1] = key;
	}
}

void
uppercase(int num_args, char *arr[])
{
	int i = 0;
	char *p;

	for (i = 1; i < num_args; i++) {
		p = arr[i];

		while (*p != '\0') {

			if (*p >= 'a' && *p <= 'z') {
				*p -= UPPERCASE_CONST;
			}
			p++;
		}
	}
}

int
main(int argc, char *argv[])
{
	int j;

	if (argc == 1) {
		fprintf(stderr, "error: no arguments\n");
		exit(EXIT_FAILURE);
	}

	uppercase(argc, argv);

	insertion_sort(&argv[1], argc - 1);

	/* Print without duplicates */
	/* Print the first element on the list */
	printf("%s\n", argv[1]);

	/* Print comparing the following 2 elements */
	for (j = 2; j < argc; j++) {

		if (strcmp(argv[j], argv[j - 1]) != 0) {
			printf("%s\n", argv[j]);
		}
	}

	exit(EXIT_SUCCESS);
}
