#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

void lib(char replace_from, char replace_to, char* input_filename, char* output_filename) {

    char letter;
    FILE *input = fopen(input_filename, "r");
    FILE *output = fopen(output_filename, "w");

    if(input == NULL) {
        printf("Input file open error");
    }

    else if(output == NULL) {
        printf("Output file open error");
    }

    else {
        while(fread(&letter, 1, 1, input)) {
            if(letter == replace_from) letter = replace_to;
            fwrite(&letter, 1, 1, output);
        }
    }
    
    fclose(input);
    fclose(output);
}

void sys(char replace_from, char replace_to, char* input_filename, char* output_filename) {

    char letter;
    int input = open(input_filename, O_RDONLY);
    int output = open(output_filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

    while(read(input, &letter, 1)) {
        if(letter == replace_from) letter = replace_to;
        write(output, &letter, 1);
    }

}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    lib(argv[1][0], argv[2][0], argv[3], argv[4]);

    clock_gettime(CLOCK_REALTIME, &end);

    long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("Lib execution time: %ld ns\n", real_time);


    clock_gettime(CLOCK_REALTIME, &start);

    sys(argv[1][0], argv[2][0], argv[3], argv[4]);

    clock_gettime(CLOCK_REALTIME, &end);

    real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("Sys execution time: %ld ns\n", real_time);

    return 0;
}
