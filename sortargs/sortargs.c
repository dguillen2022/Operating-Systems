#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(char *arr[], int n)
{
    for (int i = 1; i < n; i++) {
        char *key = arr[i];
        int j = i - 1;

        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int
main(int argc, char *argv[])
{
    int i = 0;
    char *p;

    if (argc == 1) {
        fprintf(stderr, "Introduce cadenas de texto\n");
        exit(EXIT_FAILURE);
    }

    // Convertir en mayúsculas
    for (i = 1; i < argc; i++) {
        p = argv[i];

        while (*p != '\0') {
            if (*p >= 'a' && *p <= 'z') {
                *p -= 32;
            }
            p++;
        }
    }

    insertion_sort(&argv[1], argc - 1);

    printf("%s\n", argv[1]);

    for (i = 2; i < argc; i++) {
        if (strcmp(argv[i], argv[i-1]) != 0) {
            printf("%s\n", argv[i]);
        }
    }

    exit(EXIT_SUCCESS);
}