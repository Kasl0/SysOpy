#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }
    
    setbuf(stdout, NULL);
    printf("%s ", argv[0]);

    execl("/bin/ls", "ls", "-l", argv[1], NULL);
    
    return 0;
}
