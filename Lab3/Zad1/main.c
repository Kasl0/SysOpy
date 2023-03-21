#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }
    
    int no_processes = atoi(argv[1]);
    pid_t pid;

    for (int i = 0; i < no_processes; i++) {
        pid = fork();
        if (pid == 0) {
            printf("Identyfikator procesu macierzystego: %d i swój własny: %d\n", (int)getppid(), (int)getpid());
            break;
        }
    }

    while(wait(0) > 0);
    if (pid != 0) printf("%d\n", no_processes);
    
    return 0;
}
