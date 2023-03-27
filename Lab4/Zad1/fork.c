#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("%d signal received\n", signum);
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }


    if (strcmp( argv[1], "ignore" ) == 0) {
        signal(SIGUSR1, SIG_IGN);

        raise(SIGUSR1);

        pid_t pid = fork();
        if (pid == 0) {
            raise(SIGUSR1);
        }
    }

    else if (strcmp( argv[1], "handler" ) == 0) {
        signal(SIGUSR1, handler);

        raise(SIGUSR1);

        pid_t pid = fork();
        if (pid == 0) {
            raise(SIGUSR1);
        }
    }

    else if (strcmp( argv[1], "mask" ) == 0) {
        sigset_t newmask, oldmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        raise(SIGUSR1);

        pid_t pid = fork();
        if (pid == 0) {
            raise(SIGUSR1);
        }
    }

    else if (strcmp( argv[1], "pending" ) == 0) {
        sigset_t newmask, oldmask, set;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        raise(SIGUSR1);

        sigpending(&set);
        if (sigismember(&set, SIGUSR1) == 1) printf("SIGUSR1 is pending\n");

        pid_t pid = fork();
        if (pid == 0) {
            sigpending(&set);
            if (sigismember(&set, SIGUSR1) == 1) printf("SIGUSR1 is pending\n");
        }
    }

    else {
        printf("Invalid command\n");
    }

    return 0;
}
