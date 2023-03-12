#include <stdio.h>
#include <ftw.h>

#define NOPEND 1000

long long total_size = 0;

int wc_file(const char *path, const struct stat *status, int typeflag) {
    if(!S_ISDIR(status -> st_mode)) {
        printf("%ld %s\n", status -> st_size, path);
        total_size += status -> st_size;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }
    
    ftw(argv[1], wc_file, NOPEND);

    printf("Total size: %lld\n", total_size);
    return 0;
}
