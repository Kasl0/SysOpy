#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int mode = 0;
int requests = 0;

void print_current_time() {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    printf("Current time: %d:%d:%d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
}

void handler(int signum, siginfo_t *si, void *p3) {

    printf("Signal number: %d, Signal PID: %d, Value: %d\n", signum, si -> si_pid, si -> si_value.sival_int);
    
    mode = si -> si_value.sival_int;
    requests++;
    
    kill(si -> si_pid, SIGUSR1);
}

int main(void) {

    printf("Pid: %d\n", getpid());

    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act, NULL);

    while(1) {

        switch (mode) {

            case 1:
                for (int i=1; i<=100; i++) printf("%d ", i);
                printf("\n");
                mode = 0;
                break;

            case 2:
                print_current_time();
                mode = 0;
                break;

            case 3:
                printf("Received requests: %d\n", requests);
                mode = 0;
                break;
            
            case 4:
                while(mode == 4) {
                    print_current_time();
                    sleep(1);
                }
                break;

            case 5:
                return 0;
                break;

            default:
                sleep(1);
                break;
        }
        
    }

    return 0;
}
