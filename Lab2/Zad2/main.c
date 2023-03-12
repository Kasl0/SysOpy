#include <stdio.h>
#include <time.h>
#include <string.h>

void copy_1(char* input_filename, char* output_filename) {

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
        fseek(input, -1, SEEK_END);
        fread(&letter, 1, 1, input);
        fwrite(&letter, 1, 1, output);

        while(fseek(input, -2, SEEK_CUR) == 0) {
            fread(&letter, 1, 1, input);
            fwrite(&letter, 1, 1, output);
        }
    }

    fclose(input);
    fclose(output);
}

void reverse_string(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char tmp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = tmp;
    }
}

void copy_1024(char* input_filename, char* output_filename) {

    char buffer[1024];
    FILE *input = fopen(input_filename, "r");
    FILE *output = fopen(output_filename, "w");

    if(input == NULL) {
        printf("Input file open error");
    }

    else if(output == NULL) {
        printf("Output file open error");
    }

    else {
        int len;

        fseek(input, -1024, SEEK_END);
        len = fread(buffer, 1, 1024, input);
        reverse_string(buffer);
        fwrite(buffer, 1, len, output);

        while(fseek(input, -2048, SEEK_CUR) == 0) {
            len = fread(buffer, 1, 1024, input);
            reverse_string(buffer);
            fwrite(buffer, 1, len, output);
        }
    }

    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    copy_1(argv[1], argv[2]);

    clock_gettime(CLOCK_REALTIME, &end);

    long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("Copy each char execution time: %ld ns\n", real_time);


    clock_gettime(CLOCK_REALTIME, &start);

    copy_1024(argv[1], argv[2]);

    clock_gettime(CLOCK_REALTIME, &end);

    real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("Copy 1024 chars execution time: %ld ns\n", real_time);

    return 0;
}
