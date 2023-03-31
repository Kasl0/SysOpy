#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2 && argc != 4) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "nadawca") == 0) {

        FILE *f;
        char buf[1024];

        f = popen("mail", "r");
        while (fgets(buf, sizeof(buf), f) != NULL) printf("%s", buf);
        pclose(f);
    }

    else if (argc == 2 && strcmp(argv[1], "data") == 0) {

        FILE *f;
        char buf[1024];

        f = popen("mail", "r");
        while (fgets(buf, sizeof(buf), f) != NULL) printf("%s", buf);
        pclose(f);
    }

    else if (argc == 4) {

        char str[100];
        sprintf(str, "mail -s \"%s\" %s", argv[2], argv[1]);

        FILE *f;
        f = popen(str, "w");
        fputs(argv[3], f);
        pclose(f);
    }

    return 0;
}
