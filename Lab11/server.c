#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAX_CLIENTS 10
#define MSG_MAX_LEN 200

int s;

char* readFile(char* filename, long* file_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc((*file_size + 1) * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file);
        return NULL;
    }

    size_t result = fread(buffer, sizeof(char), *file_size, file);
    if (result != *file_size) {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[*file_size] = '\0';
    (*file_size)++;

    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Invalid number of arguments specified: %d\n", argc);
        return 0;
    }

    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("socket() error\n"); 
        return 0;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *) &server, sizeof server) == -1) {
        printf("bind() error\n");
        return 0;
    }

    if (listen(s, MAX_CLIENTS) == -1) {
        printf("listen() error\n");
        return 0;
    }

    int end = 0;
    int received;
    char msg[MSG_MAX_LEN];
    struct sockaddr_in client;
    int len = sizeof(client);
    int client_socket;

    while (!end) {
        
        client_socket = accept(s, (struct sockaddr*)&client, (socklen_t *) &len);
        if (client_socket == -1) {
            printf("accept() error\n");
            return 0;
        }

        received = read(client_socket, msg, sizeof(msg));
        msg[received] = '\0';
        char* word = strtok(msg, ": \n");

        if (strcmp(word, "exit") == 0) {
            end = 1;
            strcpy(msg, "exit");
            write(client_socket, msg, strlen(msg));
        }
        else if (strcmp(word, "podaj") == 0) {

            char* word = strtok(NULL, ": \n");

            long file_size;
            char* content = readFile(word, &file_size);

            sprintf(msg, "size:%ld:", file_size);
            write(client_socket, msg, strlen(msg));
            write(client_socket, content, file_size);
        }
        else printf("Unknown command: %s", word);

        close(client_socket);
    }

    close(s);

    return 0;
}
