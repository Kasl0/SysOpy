#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int main(void)
{
    long long total_size = 0;

    DIR* dir = opendir("./");

    struct dirent* file;
    struct stat status;

    file = readdir(dir);
    while(file) {

        stat(file -> d_name, &status);
        if(!S_ISDIR(status.st_mode)) {

            printf("%ld %s\n", status.st_size, file -> d_name);
            total_size += status.st_size;
        }
        
        file = readdir(dir);
    }
    
    closedir(dir);

    printf("Total size: %lld\n", total_size);
    return 0;
}
