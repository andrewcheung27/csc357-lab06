#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


int main(int argc, char *argv[]) {
    long seconds;
    char *c;

    /* not enough args */
    if (argc < 2) {
        fprintf(stderr,
                "Missing arg.\nUsage: timeit seconds\n");
        exit(EXIT_FAILURE);
    }
    /* too many args */
    if (argc > 2) {
        fprintf(stderr,
                "Too many args.\nUsage: timeit seconds\n");
        exit(EXIT_FAILURE);
    }
    c = NULL;
    seconds = strtol(argv[1], &c, 0);
    /* needs an int */
    if (*c || seconds < 0) {
        fprintf(stderr,
                "Argument must be an integer.\nUsage: timeit seconds\n");
        exit(EXIT_FAILURE);
    }

    /* timer stuff */
    printf("seconds = %ld\n", seconds);
    fflush(stdout);

    return 0;
}

