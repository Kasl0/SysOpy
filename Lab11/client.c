#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int s;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 0;
    }

    struct sockaddr_in server;
    int status;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("socket() error\n");
        return 0;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    status = connect(s, (struct sockaddr*)&server, sizeof(server));
    if (status < 0) {
        printf("connect() error\n");
        return 0;
    }

    char buf[200];
    printf("Podaj tekst: ");
    fgets(buf, sizeof(buf), stdin);

    write(s, buf, strlen(buf));

    status = read(s, buf, (sizeof buf)-1);
    if(status > 0) {

        buf[status] = '\0';
        char* word = strtok(buf, ": \n");

        if(strcmp(word, "exit") == 0) printf("Server closed\n");
        else if(strcmp(word, "size") == 0) {

            FILE* file = fopen("received.txt", "a");
            if (file == NULL) {
                fprintf(stderr, "Failed to open file\n");
            }

            word = strtok(NULL, ": \n");

            long file_size = atoi(word);

            word = strtok(NULL, "\0");
            file_size -= 1;

            fprintf(file, "%s", word);

            file_size -= strlen(word);

            while(file_size > 0) {
                status = read(s, buf, (sizeof buf)-1);
                buf[status] = '\0';
                fprintf(file, "%s", buf);
                file_size -= status;
            }
            
            printf("Zapisano plik\n");
            fclose(file);
        }
        else printf("Unknown content: %s\n", word);
    }

    close(s);

    return 0;
}
