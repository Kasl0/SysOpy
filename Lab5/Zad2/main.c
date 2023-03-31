#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


double function(double x) {
    return 4 / (x*x + 1);
}


double rectangle(double x1, double x2) {
    return (x2 - x1) * (function(x1) + function(x2)) / 2;
}


double process(int i, double width, int n) {

    double sum = 0;
    double x = (double) i / n;
    
    while(x + width <= (double) (i + 1) / n) {
        sum += rectangle(x, x + width);
        x += width;
    }

    sum += rectangle(x, (double) (i + 1) / n);

    
    return sum;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    double width = strtod(argv[1], NULL);
    int n = atoi(argv[2]);
    
    pid_t pid;
    int fd[n][2];
    double val;

    for (int i = 0; i < n; i++) {

        pipe(fd[i]);
        pid = fork();

        if(pid == 0) {

            close(fd[i][0]);

            val = process(i, width, n);
            write(fd[i][1], &val, sizeof(double));

            return 0;
        }
        close(fd[i][1]);
    }

    double sum = 0;
    for (int i = 0; i < n; i++) {
        read(fd[i][0], &val, sizeof(double));
        sum += val;
    }
    
    printf("Sum: %f\n", sum);

    clock_gettime(CLOCK_REALTIME, &end);

    double real_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 0.000000001;
    printf("Execution time: %f s\n", real_time);

    return 0;
}
