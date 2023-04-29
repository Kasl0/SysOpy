#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>
#include "common.h"

mqd_t server_queue;
mqd_t clients[MAX_CLIENTS] = {0};

void create_server_queue();
void open_client_queue(char* client_queue_name);
void remove_server_queue_and_exit();
void send_end();
int get_new_id();
void send_list(int id);
void send_one(int sender_id, int id, char* msg, long type);
void send_all(int sender_id, char* msg);
void msg_interpreter(struct msgbuf msg, int type);

int main(void) {
    mq_unlink(SERVER_QUEUE_NAME);

    setbuf(stdout, NULL);

    signal(SIGINT, send_end);

    create_server_queue();

    while(1) {
        struct msgbuf msg;
        unsigned int type;
        
        if (mq_receive(server_queue, (char*)&msg, MSGSZ, &type) == -1) {
            perror("mq_receive");
            continue;
        }

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char datetime[20];
        
        strftime(datetime, sizeof(datetime), "%H:%M %d.%m.%Y", tm);

        FILE *fp = fopen("log.txt", "a");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        fprintf(fp, "%s;%d;%s\n", datetime, msg.id, msg.mtext);
        
        fclose(fp);

        msg_interpreter(msg, type);
    }

    return 0;

}

void create_server_queue() {
    
    struct mq_attr atr;
	atr.mq_flags = 0;
	atr.mq_maxmsg = MAX_CLIENTS;
	atr.mq_msgsize = MSGSZ;
	atr.mq_curmsgs = 0;
	
	server_queue = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &atr);
    if (server_queue == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Server queue created\n");
}

void open_client_queue(char* client_queue_name) {
    printf("Received message INIT, client queue name: %s\n", client_queue_name);

    int new_id = get_new_id();
    if (new_id == -1) {
        printf("Maximum number of clients has already been reached\n");
        exit(EXIT_FAILURE);
    }
    printf("Client ID: %d\n", new_id);

    clients[new_id] = mq_open(client_queue_name, O_WRONLY);
    if (clients[new_id] == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    struct msgbuf msg;
    snprintf(msg.mtext, sizeof(msg.mtext), "%d", new_id);

    if (mq_send(clients[new_id], (const char *)&msg, MSGSZ, INIT_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
}

void remove_server_queue_and_exit() {
    if (server_queue > 0) {
        if (mq_close(server_queue) == -1) {
            perror("mq_close");
        }
        mq_unlink(SERVER_QUEUE_NAME);
        printf("Closed and removed server queue\n");
        server_queue = 0;
    }
    exit(0);
}

void send_end() {

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {

            struct msgbuf msg;

            if (mq_send(clients[i], (const char *)&msg, MSGSZ, STOP_MSG) == -1) {
                perror("mq_send");
                exit(EXIT_FAILURE);
            }

            printf("Sent STOP_MSG to %d\n", i);

            struct msgbuf msg_stop;

            if (mq_receive(server_queue, (char*)&msg_stop, MSGSZ, NULL) == -1) {
                perror("mq_receive");
                continue;
            }

            printf("Received message STOP\n");

            if (clients[msg_stop.id] > 0) {
                if (mq_close(clients[msg_stop.id]) == -1) {
                    perror("mq_close");
                }
                printf("Closed client queue\n");
                clients[msg_stop.id] = 0;
            }

        }
    }

    remove_server_queue_and_exit();
}

int get_new_id() {
    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] == 0) return i;
    }
    return -1;
}

void send_list(int id) {
    printf("Received message LIST\n");

    struct msgbuf msg;
    strcpy(msg.mtext, "");

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {

            if (strcmp(msg.mtext, "") != 0) {
                strcat(msg.mtext, ", ");
            }

            char num[10];
            snprintf(num, 10, "%d", i);

            strcat(msg.mtext, num);
        }
    }

    if (mq_send(clients[id], (const char *)&msg, MSGSZ, LIST_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Sent: %s\n", msg.mtext);
}

void send_one(int sender_id, int id, char* text, long type) {
    struct msgbuf msg;
    strcpy(msg.mtext, text);
    msg.id = sender_id;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(msg.tm, sizeof(msg.tm), "%H:%M %d.%m.%Y", tm);

    if (mq_send(clients[id], (const char *)&msg, MSGSZ, type) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    printf("Sent: %s to %d\n", msg.mtext, id);
}

void send_all(int sender_id, char* msg) {
    printf("Received message 2ALL %s\n", msg);

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {
            send_one(sender_id, i, msg, ALL_MSG);
        }
    }
}

void msg_interpreter(struct msgbuf msg, int type) {
    switch (type) {

        case STOP_MSG:
            printf("Received message STOP\n");
            if (clients[msg.id] > 0) {
                if (mq_close(clients[msg.id]) == -1) {
                    perror("mq_close");
                }
                printf("Closed client queue\n");
                clients[msg.id] = 0;
            }
            break;

        case LIST_MSG:
            send_list(msg.id);
            break;

        case INIT_MSG:
            open_client_queue(msg.mtext);
            break;

        case ALL_MSG:
            send_all(msg.id, msg.mtext);
            break;

        case ONE_MSG:
            printf("Received message 2ONE %d %s\n", msg.receiver, msg.mtext);
            send_one(msg.id, msg.receiver, msg.mtext, ONE_MSG);
            break;

        default:
            printf("Received message with unknown type: %d\n", type);
            break;
    }
}
