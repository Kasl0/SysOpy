#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
    printf("Signal number: %d\n", signum);
    sleep(1);
}

int main() {

    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;

    sigaction(SIGINT, &act, NULL);

    while(1) {
        printf(">\n");
        sleep(3);
    }

    return 0;
}
