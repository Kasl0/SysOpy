#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <dlfcn.h>

#define MAX_CMD_LEN 50

struct results {
    char** result_array;
    int size;
    int taken;
};

struct results r;

int main(int argc, char* argv[]) {

    void *handle = dlopen("./libwc.so", RTLD_LAZY);
    if(!handle) {
        printf("Library open error");
        return 0;
    }

    struct results (*init)(int);
    init = dlsym(handle, "init");
    if(dlerror() != NULL) {
        printf("Init function error");
        return 0;
    }

    void (*wc)(struct results*, const char*);
    wc = dlsym(handle, "wc");
    if(dlerror() != NULL) {
        printf("Wc function error");
        return 0;
    }

    char* (*return_result)(struct results*, int);
    return_result = dlsym(handle, "return_result");
    if(dlerror() != NULL) {
        printf("Return_result function error");
        return 0;
    }

    void (*delete_result)(struct results*, int);
    delete_result = dlsym(handle, "delete_result");
    if(dlerror() != NULL) {
        printf("Delete_result function error");
        return 0;
    }

    void (*delete_result_array)(struct results*);
    delete_result_array = dlsym(handle, "delete_result_array");
    if(dlerror() != NULL) {
        printf("Delete_result_array function error");
        return 0;
    }
    
    char command[MAX_CMD_LEN];

    while(1) {

        if(fgets(command, MAX_CMD_LEN, stdin) == NULL) break;

        char command_copy[MAX_CMD_LEN];
        strcpy(command_copy, command);
        char *word = strtok(command, " ");

        if(strcmp(word, "init") == 0) {
            printf("> %s", command_copy);

            word = strtok(NULL, "\n");

            struct timespec start, end;
            struct tms tms_start, tms_end;

            clock_gettime(CLOCK_REALTIME, &start);
            times(&tms_start);

            r = init(atoi(word));

            clock_gettime(CLOCK_REALTIME, &end);
            times(&tms_end);

            long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            long int user_time = (tms_end.tms_utime - tms_start.tms_utime) * 1000000000 / CLOCKS_PER_SEC;
            long int system_time = (tms_end.tms_stime - tms_start.tms_stime) * 1000000000 / CLOCKS_PER_SEC;

            printf("Real time: %ld ns\n", real_time);
            printf("User time: %ld ns\n", user_time);
            printf("System time: %ld ns\n", system_time);
        }

        else if(strcmp(word, "count") == 0) {
            printf("> %s", command_copy);

            word = strtok(NULL, "\n");

            struct timespec start, end;
            struct tms tms_start, tms_end;

            clock_gettime(CLOCK_REALTIME, &start);
            times(&tms_start);

            wc(&r, word);

            clock_gettime(CLOCK_REALTIME, &end);
            times(&tms_end);

            long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            long int user_time = (tms_end.tms_utime - tms_start.tms_utime) * 1000000000 / CLOCKS_PER_SEC;
            long int system_time = (tms_end.tms_stime - tms_start.tms_stime) * 1000000000 / CLOCKS_PER_SEC;

            printf("Real time: %ld ns\n", real_time);
            printf("User time: %ld ns\n", user_time);
            printf("System time: %ld ns\n", system_time);
        }

        else if(strcmp(word, "show") == 0) {
            printf("> %s", command_copy);

            word = strtok(NULL, "\n");

            struct timespec start, end;
            struct tms tms_start, tms_end;

            clock_gettime(CLOCK_REALTIME, &start);
            times(&tms_start);
            
            printf("%s",return_result(&r, atoi(word)));

            clock_gettime(CLOCK_REALTIME, &end);
            times(&tms_end);

            long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            long int user_time = (tms_end.tms_utime - tms_start.tms_utime) * 1000000000 / CLOCKS_PER_SEC;
            long int system_time = (tms_end.tms_stime - tms_start.tms_stime) * 1000000000 / CLOCKS_PER_SEC;

            printf("Real time: %ld ns\n", real_time);
            printf("User time: %ld ns\n", user_time);
            printf("System time: %ld ns\n", system_time);
        }

        else if(strcmp(word, "delete") == 0) {
            printf("> %s", command_copy);

            word = strtok(NULL, "\n");

            struct timespec start, end;
            struct tms tms_start, tms_end;

            clock_gettime(CLOCK_REALTIME, &start);
            times(&tms_start);

            delete_result(&r, atoi(word));

            clock_gettime(CLOCK_REALTIME, &end);
            times(&tms_end);

            long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            long int user_time = (tms_end.tms_utime - tms_start.tms_utime) * 1000000000 / CLOCKS_PER_SEC;
            long int system_time = (tms_end.tms_stime - tms_start.tms_stime) * 1000000000 / CLOCKS_PER_SEC;

            printf("Real time: %ld ns\n", real_time);
            printf("User time: %ld ns\n", user_time);
            printf("System time: %ld ns\n", system_time);
        }

        else if(strcmp(word, "destroy\n") == 0) {
            printf("> %s", command_copy);

            struct timespec start, end;
            struct tms tms_start, tms_end;

            clock_gettime(CLOCK_REALTIME, &start);
            times(&tms_start);

            delete_result_array(&r);

            clock_gettime(CLOCK_REALTIME, &end);
            times(&tms_end);

            long int real_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            long int user_time = (tms_end.tms_utime - tms_start.tms_utime) * 1000000000 / CLOCKS_PER_SEC;
            long int system_time = (tms_end.tms_stime - tms_start.tms_stime) * 1000000000 / CLOCKS_PER_SEC;

            printf("Real time: %ld ns\n", real_time);
            printf("User time: %ld ns\n", user_time);
            printf("System time: %ld ns\n", system_time);
        }

        else if(strcmp(word, "\n") == 0) {
            continue;
        }
        
        else {
            printf("Unknown command: %s", command);
        }
    }

    dlclose(handle);
    return 0;
}
