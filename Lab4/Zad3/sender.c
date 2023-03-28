#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void obsluga(int signum) {
    printf("Confirmation received\n");
}

void send(int pid, int value) {
    union sigval sigvalue;
    sigvalue.sival_int = value;
    sigqueue(pid, SIGUSR1, sigvalue);
}

int main(int argc, char *argv[]) {

    sigset_t set;
    sigemptyset(&set);

    signal(SIGUSR1, obsluga);

    if (argc < 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    int pid = atoi(argv[1]);

    for (int i=2; i<argc; i++) {
        send(pid, atoi(argv[i]));
        sigsuspend(&set);
    }

}
