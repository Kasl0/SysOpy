#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

void new_directory(char* directory_path, char* search_string) {

    pid_t pid = fork();

    if (pid == 0) {

        DIR* dir = opendir(directory_path);
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        struct dirent* file;
        char filename_path[PATH_MAX];
        struct stat status;

        file = readdir(dir);
        while(file) {

            if (strcmp(file -> d_name, ".") != 0 && strcmp(file -> d_name, "..") != 0) {

                strcpy(filename_path, directory_path);
                strcat(filename_path, "/");
                strcat(filename_path, file -> d_name);
                if (stat(filename_path, &status) == -1) {
                    perror("stat");
                    exit(EXIT_FAILURE);
                }

                if (S_ISDIR(status.st_mode)) new_directory(filename_path, search_string);

                else if (S_ISREG(status.st_mode)) {

                    FILE *file = fopen(filename_path, "r");
                    if (file == NULL) {
                        perror("fopen");
                        continue;
                    }

                    char line[255];
                    if (fgets(line, 255, file) != NULL && strstr(line, search_string) == line) {
                        printf("%s %d\n", filename_path, getpid());
                    }

                    fclose(file);
                }

            }

            file = readdir(dir);
        }
        
        if (closedir(dir) == -1) {
            perror("closedir");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    new_directory(argv[1], argv[2]);
    
    return 0;
}
