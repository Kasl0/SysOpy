#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum, siginfo_t *si, void *p3) {
    printf("Signal number: %d, Signal UID: %d, Signal PID: %d\n", signum, si -> si_uid, si -> si_pid);

    char **str_ptr = (char **)si -> si_value.sival_ptr;

    for(int i=0; i<3; i++) {
        printf("%s\n", str_ptr[i]);
    }
}

int main() {

    char *str[3] = {"info1", "info2", "info3"};

    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act, NULL);

    pid_t pid = fork();
    if (pid == 0) {

        sleep(5);

        union sigval value;
        value.sival_ptr = str;

        sigqueue(getppid(), SIGUSR1, value);

        exit(EXIT_SUCCESS);
    }

    while(1) {
        printf(">\n");
        sleep(3);
    }

    return 0;
}
