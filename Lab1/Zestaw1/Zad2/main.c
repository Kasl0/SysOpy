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

extern struct results init(int);
extern void wc(struct results*, const char*);
extern char* return_result(struct results*, int);
extern void delete_result(struct results*, int);
extern void delete_result_array(struct results*);

struct results (*init2)(int);
void (*wc2)(struct results*, const char*);
char* (*return_result2)(struct results*, int);
void (*delete_result2)(struct results*, int);
void (*delete_result_array2)(struct results*);

struct results r;

int main(int argc, char* argv[]) {

    if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) {

        void *handle = dlopen("Zad1/libwc.so", RTLD_LAZY);
        if(!handle) {
            printf("Library open error");
            return 0;
        }

        init2 = dlsym(handle, "init");
        if(dlerror() != NULL) {
            printf("Init function error");
            return 0;
        }

        wc2 = dlsym(handle, "wc");
        if(dlerror() != NULL) {
            printf("Wc function error");
            return 0;
        }

        return_result2 = dlsym(handle, "return_result");
        if(dlerror() != NULL) {
            printf("Return_result function error");
            return 0;
        }

        delete_result2 = dlsym(handle, "delete_result");
        if(dlerror() != NULL) {
            printf("Delete_result function error");
            return 0;
        }

        delete_result_array2 = dlsym(handle, "delete_result_array");
        if(dlerror() != NULL) {
            printf("Delete_result_array function error");
            return 0;
        }

        dlclose(handle);
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

            if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) r = init2(atoi(word));
            else r = init(atoi(word));

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

            if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) wc2(&r, word);
            else wc(&r, word);

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
            
            if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) printf(return_result2(&r, atoi(word)));
            else printf(return_result(&r, atoi(word)));

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

            if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) delete_result2(&r, atoi(word));
            else delete_result(&r, atoi(word));

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

            if(argc > 0 && strcmp(argv[0], "./Zad2/main_dynamic") == 0) delete_result_array2(&r);
            else delete_result_array(&r);

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

    return 0;
}
