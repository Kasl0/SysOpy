#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "common.h"

int msgid;
int clients[MAX_CLIENTS] = {0};

void create_server_queue();
void open_client_queue(int key);
void remove_server_queue_and_exit();
void send_end();
int get_new_id();
void send_list(int id);
void send_one(int sender_id, int id, char* msg, long type);
void send_all(int sender_id, char* msg);
void msg_interpreter(struct msgbuf msg);

int main(void) {

    setbuf(stdout, NULL);

    signal(SIGINT, send_end);

    create_server_queue();

    while(1) {
        struct msgbuf msg;

        if (msgrcv(msgid, &msg, MSGSZ, -100, 0) == -1) {
            perror("msgrcv");
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

        msg_interpreter(msg);
    }

    return 0;

}

void create_server_queue() {
    printf("Server key: %d\n", SERVER_QUEUE_KEY);

    msgid = msgget(SERVER_QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Server queue created, ID: %d\n", msgid);
}

void open_client_queue(int key) {
    printf("Received message INIT, client key: %d\n", key);

    int new_id = get_new_id();
    if (new_id == -1) {
        printf("Maximum number of clients has already been reached\n");
        exit(EXIT_FAILURE);
    }

    clients[new_id] = msgget(key, 0666);
    if (clients[new_id] == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Client queue ID: %d\n", clients[new_id]);

    struct msgbuf msg_id;
    msg_id.mtype = INIT_MSG;
    snprintf(msg_id.mtext, sizeof(msg_id.mtext), "%d", new_id);

    if (msgsnd(clients[new_id], &msg_id, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void remove_server_queue_and_exit() {
    if (msgid > 0) {
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
        }
        printf("Removed queue %d\n", msgid);
        msgid = 0;
    }
    exit(0);
}

void send_end() {

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {

            struct msgbuf msg;
            msg.mtype = STOP_MSG;

            if (msgsnd(clients[i], &msg, MSGSZ, 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            printf("Sent STOP_MSG to %d\n", i);

            struct msgbuf msg_stop;

            if (msgrcv(msgid, &msg_stop, MSGSZ, STOP_MSG, 0) == -1) {
                perror("msgrcv");
                continue;
            }

            printf("Received message STOP\n");
            clients[msg_stop.id] = 0;

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

    struct msgbuf msg_list;
    msg_list.mtype = LIST_MSG;
    strcpy(msg_list.mtext, "");

    for (int i=0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {

            if (strcmp(msg_list.mtext, "") != 0) {
                strcat(msg_list.mtext, ", ");
            }

            char num[10];
            snprintf(num, 10, "%d", i);

            strcat(msg_list.mtext, num);
        }
    }

    if (msgsnd(clients[id], &msg_list, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Sent: %s\n", msg_list.mtext);
}

void send_one(int sender_id, int id, char* text, long type) {
    struct msgbuf msg;
    msg.mtype = type;
    strcpy(msg.mtext, text);
    msg.id = sender_id;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(msg.tm, sizeof(msg.tm), "%H:%M %d.%m.%Y", tm);

    if (msgsnd(clients[id], &msg, MSGSZ, 0) == -1) {
        perror("msgsnd");
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

void msg_interpreter(struct msgbuf msg) {
    switch (msg.mtype) {

        case STOP_MSG:
            printf("Received message STOP\n");
            clients[msg.id] = 0;
            break;

        case LIST_MSG:
            send_list(msg.id);
            break;

        case INIT_MSG:
            open_client_queue(atoi(msg.mtext));
            break;

        case ALL_MSG:
            send_all(msg.id, msg.mtext);
            break;

        case ONE_MSG:
            printf("Received message 2ONE %d %s\n", msg.receiver, msg.mtext);
            send_one(msg.id, msg.receiver, msg.mtext, ONE_MSG);
            break;

        default:
            printf("Received message with unknown type: %ld\n", msg.mtype);
            break;
    }
}
