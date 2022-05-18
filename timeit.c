/* count down program */


#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


long getSeconds(int argc, char *argv[]) {
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

    return seconds;
}


/* timer that goes off every half-second */
static int setTimer(struct itimerval *timer) {
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = 500000;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = 500000;

    setitimer(ITIMER_REAL, timer, 0);
    return 0;
}


/* signal handler that prints tick tock */
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
    int_fast64_t doubledSeconds;
    sigset_t set;
    sigset_t oldset;
    struct itimerval timer;
    struct sigaction sa;

    /* block interrupts */
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, &oldset);

    /* read number of seconds from command line args */
    seconds = getSeconds(argc, argv);

    /* register handler for SIGALRM */
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handler;
    sigaction(SIGALRM, &sa, 0);

    /* set timer to send SIGALRM every half-second */
    setTimer(&timer);

    /* seconds doubled because we have two signals per second */
    doubledSeconds = seconds;
    doubledSeconds *= 2;
    while (doubledSeconds--) {
        pause();
    }

    printf("Time's up!\n");
    sigprocmask(SIG_UNBLOCK, &set, 0);
    sigprocmask(SIG_BLOCK, &oldset, 0);
    return 0;
}

