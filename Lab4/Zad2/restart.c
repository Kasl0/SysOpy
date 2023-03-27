#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
    printf("Signal number: %d\n", signum);
}

int main() {

    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    sigaction(SIGINT, &act, NULL);

    while (1) {

        char buffer[10];
        fgets(buffer, 10, stdin);
        printf("Buffer: %s\n", buffer);

        raise(SIGINT);
    }

    return 0;
}
