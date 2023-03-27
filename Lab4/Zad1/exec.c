#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }


    if (strcmp( argv[1], "ignore" ) == 0) {
        signal(SIGUSR1, SIG_IGN);

        raise(SIGUSR1);

        execl("/bin/ls", "ls", ".", NULL);

        raise(SIGUSR1);
    }

    else if (strcmp( argv[1], "mask" ) == 0) {
        sigset_t newmask, oldmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        raise(SIGUSR1);

        execl("/bin/ls", "ls", ".", NULL);

        raise(SIGUSR1);
    }

    else if (strcmp( argv[1], "pending" ) == 0) {
        sigset_t newmask, oldmask, set;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        raise(SIGUSR1);

        sigpending(&set);
        if (sigismember(&set, SIGUSR1) == 1) printf("SIGUSR1 is pending\n");

        execl("/bin/ls", "ls", ".", NULL);
        
        sigpending(&set);
        if (sigismember(&set, SIGUSR1) == 1) printf("SIGUSR1 is pending\n");
        
    }

    else {
        printf("Invalid command\n");
    }

    return 0;
}
