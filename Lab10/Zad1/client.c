#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

int s;
int id;

void receive_id() {

    char buf[MSG_MAX_LEN];
    read(s, buf, (sizeof buf)-1);
    
    id = atoi(buf);
    printf("Received ID: %d\n", id);
}

void send_stop() {
    if (s > 0) {
        char msg[MSG_MAX_LEN];
        sprintf(msg, "STOP;%d\n", id);
        write(s, msg, strlen(msg));
        printf("Sent STOP\n");
        close(s);
        printf("Socket closed\n");
    }
    exit(0);
}

void send_list() {
    if (s > 0) {
        char msg[MSG_MAX_LEN];
        sprintf(msg, "LIST;%d\n", id);
        write(s, msg, strlen(msg));
        printf("Sent LIST\n");
    }
}

void send_2all(char* text) {
    if (s > 0) {
        char msg[MSG_MAX_LEN];
        sprintf(msg, "ALL;%d;%s\n", id, text);
        write(s, msg, strlen(msg));
        printf("Sent ALL;%d;%s\n", id, text);
    }
}

void send_2one(int receiver, char* text) {
    if (s > 0) {
        char msg[MSG_MAX_LEN];
        sprintf(msg, "ONE;%d;%d;%s\n", id, receiver, text);
        write(s, msg, strlen(msg));
        printf("Sent ONE;%d;%d;%s\n", id, receiver, text);
    }
}

void msg_receiver() {
    while(1) {

        char msg[MSG_MAX_LEN] = "";
        int st = read(s, msg, (sizeof msg)-1);
        if( st > 0) {

            msg[st] = '\0';

            char *word = strtok(msg, ";");

            if(strcmp(word, "STOP\n") == 0) {
                printf("Received STOP\n");
                kill(getppid(), SIGINT);
                exit(0);
            }

            else if(strcmp(word, "PING\n") == 0) {
                printf("Received PING\n");
                write(s, "OK\n", strlen("OK\n"));
            }

            else if(strcmp(word, "LIST") == 0) {
                word = strtok(NULL, "\n");
                printf("Received LIST: %s\n", word);
            }

            else if(strcmp(word, "ALL") == 0) {
                word = strtok(NULL, "\n");
                printf("Received ALL: %s\n", word);
            }

            else if(strcmp(word, "ONE") == 0) {
                word = strtok(NULL, "\n");
                printf("Received ONE: %s\n", word);
            }

            else {
                printf("Received unknown command: %s\n", word);
            }
        }
    }
}

void cmd_interpreter(pid_t pid) {

    char command[MSG_MAX_LEN];

    while(1) {

        if(fgets(command, MSG_MAX_LEN, stdin) == NULL) send_stop();
        char *word = strtok(command, " ");

        if(strcmp(word, "STOP\n") == 0) {
            kill(pid, SIGINT);
            send_stop();
        }

        else if(strcmp(word, "LIST\n") == 0) {
            send_list();
        }

        else if(strcmp(word, "2ALL") == 0) {
            word = strtok(NULL, "\n");
            send_2all(word);
        }

        else if(strcmp(word, "2ONE") == 0) {
            int receiver = atoi(strtok(NULL, " "));
            word = strtok(NULL, "\n");
            send_2one(receiver, word);
        }

        else {
            printf("Unknown command\n");
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 4) {
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
    server.sin_port = htons(atoi(argv[3]));
    server.sin_addr.s_addr = inet_addr(argv[2]);

    status = connect(s, (struct sockaddr*)&server, sizeof(server));
    if (status < 0) {
        printf("connect() error\n");
        return 0;
    }

    receive_id();

    pid_t pid = fork();
    if (pid == 0) msg_receiver();
    else {
        signal(SIGINT, send_stop);
        cmd_interpreter(pid);
    }

    return 0;
}
