#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

double function(double x) {
    return 4 / (x*x + 1);
}

double rectangle(double x1, double x2) {
    return (x2 - x1) * (function(x1) + function(x2)) / 2;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    int i = atoi(argv[1]);
    double width = strtod(argv[2], NULL);
    int n = atoi(argv[3]);

    double sum = 0;
    double x = (double) i / n;
    
    while(x + width <= (double) (i + 1) / n) {
        sum += rectangle(x, x + width);
        x += width;
    }

    sum += rectangle(x, (double) (i + 1) / n);

    printf("%d %f %d  %f %f %f\n", i, width, n, sum, x, (double) (i + 1) / n);

    int w = open("potok", O_WRONLY);
    write(w, &sum, sizeof(double));

    return 0;
}
