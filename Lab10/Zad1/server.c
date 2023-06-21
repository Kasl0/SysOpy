#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "common.h"

int s;
int clients[MAX_CLIENTS] = {0};
int no_clients = 0;
pthread_t accept_thread, ping_thread;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_new_id() {
    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] == 0) return i;
    }
    return -1;
}

void* accept_function(void *arg) {

    struct sockaddr_in client;
    int len = sizeof(client);
    int client_socket;

    while (1) {
        
        client_socket = accept(s, (struct sockaddr*)&client, (socklen_t *) &len);
        if (client_socket == -1) {
            printf("accept() error\n");
            return 0;
        }

        pthread_mutex_lock(&clients_mutex);

        int new_id = get_new_id();
        if (new_id == -1) {
            printf("Maximum number of clients has already been reached\n");
            exit(EXIT_FAILURE);
        }
        printf("Client ID: %d\n", new_id);

        clients[new_id] = client_socket;
        no_clients++;

        char msg[MSG_MAX_LEN];
        sprintf(msg, "%d", new_id);
        write(clients[new_id], msg, strlen(msg));

        pthread_mutex_unlock(&clients_mutex);
    }
}

void* ping_function(void *arg) {

    char msg[MSG_MAX_LEN] = "PING\n";

    while(1) {

        pthread_mutex_lock(&clients_mutex);

        for(int i=0; i<MAX_CLIENTS; i++) {
            if(clients[i] != 0) {

                write(clients[i], "PING\n", strlen("PING\n"));
                printf("Sent PING to %d\n", i);

                if (read(clients[i], msg, sizeof(msg)) == 0) {
                    close(clients[i]);
                    printf("Closed client socket %d\n", i);
                    clients[i] = 0;
                    no_clients--;
                }
            }
        }

        pthread_mutex_unlock(&clients_mutex);
        
        sleep(5);
    }
}

void send_list(int id) {

    printf("Received message LIST\n");

    char msg[MSG_MAX_LEN] = "LIST;";

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {

            if (strcmp(msg, "LIST;") != 0) {
                strcat(msg, ", ");
            }

            char num[10];
            snprintf(num, 10, "%d", i);

            strcat(msg, num);
        }
    }

    write(clients[id], msg, strlen(msg));

    printf("Sent: %s\n", msg);
}

void send_one(int sender_id, int id, char* text, int to_one) {

    char msg[MSG_MAX_LEN];
    char tim[MSG_MAX_LEN];

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(tim, sizeof(tim), "%H:%M %d.%m.%Y", tm);

    if (to_one) sprintf(msg, "ONE;%d;%s;%s", sender_id, tim, text);
    else sprintf(msg, "ALL;%d;%s;%s", sender_id, tim, text);
    write(clients[id], msg, strlen(msg));

    printf("Sent: %s to %d\n", msg, id);
}

void send_all(int sender_id, char* msg) {
    printf("Received message 2ALL %s\n", msg);

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {
            send_one(sender_id, i, msg, 0);
        }
    }
}

void msg_interpreter(char* msg) {

    char *word = strtok(msg, ";");

    if(strcmp(word, "STOP") == 0) {

        printf("Received message STOP\n");

        word = strtok(NULL, "\n");
        int id = atoi(word);

        if (clients[id] > 0) {
            close(clients[id]);
            printf("Closed client socket %d\n", id);
            clients[id] = 0;
            no_clients--;
        }
    }

    else if(strcmp(word, "LIST") == 0) {

        word = strtok(NULL, "\n");
        int id = atoi(word);

        send_list(id);
    }

    else if(strcmp(word, "ALL") == 0) {

        word = strtok(NULL, ";");
        int id = atoi(word);
        word = strtok(NULL, "\n");
        send_all(id, word);
    }

    else if(strcmp(word, "ONE") == 0) {

        int id = atoi(strtok(NULL, ";"));
        int receiver = atoi(strtok(NULL, ";"));
        word = strtok(NULL, "\n");

        printf("Received message 2ONE %s\n", word);

        send_one(id, receiver, word, 1);
    }

    else {
        printf("Received unknown message: %s\n", word);
    }
}

void send_end() {

    // end accepting
    pthread_cancel(accept_thread);

    // close all client connections
    for(int i=0; i<MAX_CLIENTS; i++) {
        if(clients[i] != 0) {

            write(clients[i], "STOP\n", strlen("STOP\n"));

            printf("Sent STOP to %d\n", i);

            char msg[MSG_MAX_LEN];
            if (read(clients[i], msg, sizeof(msg)) > 0) {

                char *word = strtok(msg, ";");

                if(strcmp(word, "STOP") == 0) {

                    printf("Received message STOP\n");

                    word = strtok(NULL, "\n");
                    int id = atoi(word);

                    if (clients[id] > 0) {
                        close(clients[id]);
                        printf("Closed client socket %d\n", id);
                        clients[id] = 0;
                        no_clients--;
                    }
                }
            }
        }
    }

    // close socket
    close(s);
    printf("Closed server socket\n");

    // exit
    exit(0);
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

    pthread_create(&accept_thread, NULL, accept_function, NULL);
    pthread_create(&ping_thread, NULL, ping_function, NULL);

    signal(SIGINT, send_end);

    // read upcoming msgs
    while (1) {
        pthread_mutex_lock(&clients_mutex);

        char msg[MSG_MAX_LEN];
        for(int i=0; i<MAX_CLIENTS; i++) {
            if(clients[i] != 0) {
                if( recv(clients[i], msg, sizeof(msg), MSG_DONTWAIT) > 0) msg_interpreter(msg);
            }
        }

        pthread_mutex_unlock(&clients_mutex);
    }
}
