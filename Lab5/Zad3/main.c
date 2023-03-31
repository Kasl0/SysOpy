#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int n = atoi(argv[2]);

    mkfifo("potok", S_IFIFO | S_IRWXU);

    for (int i = 0; i < n; i++) {

        if(fork() == 0) {

            char str[100];
            sprintf(str, "%d", i);

            execl("exec", "exec", str, argv[1], argv[2], NULL);
            return 0;
        }
    }

    double sum = 0;
    double val;
    int r = open("potok", O_RDONLY);

    for (int i = 0; i < n; i++) {
        read(r, &val, sizeof(double));
        sum += val;
    }
    
    printf("Sum: %f\n", sum);

    clock_gettime(CLOCK_REALTIME, &end);

    double real_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 0.000000001;
    printf("Execution time: %f s\n", real_time);

    return 0;
}
