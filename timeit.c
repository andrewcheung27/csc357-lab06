#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


static int setTimers(struct itimerval *tick, struct itimerval *tock) {
    /* tick every second, starting immediately */
    tick->it_value.tv_sec = 0;
    tick->it_value.tv_usec = 5000000;
    tick->it_interval.tv_sec = 1;
    tick->it_interval.tv_usec = 0;

    /* tock every second, starting at a half-second offset */
    tock->it_value.tv_sec = 0;
    tock->it_value.tv_usec = 1;
    tock->it_interval.tv_sec = 1;
    tock->it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, tick, 0);
    setitimer(ITIMER_REAL, tock, 0);

    return 0;
}


void handler(int signum, siginfo_t *siginfo, void *ucontext) {
    static int n = 1;

    if (n++ % 2) {
        printf("Tick...");
    }
    else {
        printf("Tock\n");
    }

    fflush(stdout);
}


int main(int argc, char *argv[]) {
    long seconds;
    char *c;

    struct itimerval tickTimer;
    struct itimerval tockTimer;
    struct sigaction sa;


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


    /* register handler for SIGALRM */
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handler;
    sigaction(SIGALRM, &sa, 0);

    setTimers(&tickTimer, &tockTimer);

    /* possible overflow? */
    seconds *= 2;
    while (seconds--) {
        pause();
    }

    printf("Time's up!\n");
    return 0;
}

