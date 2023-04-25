#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "common.h"

key_t key;
int id;
int msgid;
int queue_id;

void open_server_queue();
void create_client_queue();
void remove_client_queue_and_exit();
void send_init();
void receive_id();
void send_stop();
void send_list();
void send_2all(char* text);
void send_2one(int receiver, char* text);
void msg_receiver();
void cmd_interpreter(pid_t pid);

int main(void) {
    
    

    open_server_queue();

    create_client_queue();

    send_init();

    receive_id();

    pid_t pid = fork();
    if (pid == 0) msg_receiver();
    else {
        signal(SIGINT, send_stop);
        cmd_interpreter(pid);
    }

    return 0;
}

void open_server_queue() {
    printf("Server key: %d\n", SERVER_QUEUE_KEY);

    msgid = msgget(SERVER_QUEUE_KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Server queue ID: %d\n", msgid);
}

void create_client_queue() {
    char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        perror("getenv");
        exit(EXIT_FAILURE);
    }

    key = ftok(home_dir, 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    int add = rand() % 10000;
    key += add;
    printf("Client key: %d\n", key);

    queue_id = msgget(key, IPC_CREAT | 0666);
    if (queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Client queue created, ID: %d\n", queue_id);
}

void remove_client_queue_and_exit() {
    if (queue_id > 0) {
        if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
            perror("msgctl");
        }
        printf("Removed queue %d\n", queue_id);
        queue_id = 0;
    }
    exit(0);
}

void send_init() {
    struct msgbuf msg;
    msg.mtype = INIT_MSG;
    msg.id = -1;
    snprintf(msg.mtext, sizeof(msg.mtext), "%d", (int) key);

    if (msgsnd(msgid, &msg, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void receive_id() {
    struct msgbuf msg;

    if (msgrcv(queue_id, &msg, MSGSZ, INIT_MSG, 0) == -1) {
        perror("msgrcv");
    }
    id = atoi(msg.mtext);
    printf("Received ID: %d\n", id);
}

void send_stop() {
    if (msgid > 0) {
        struct msgbuf msg;
        msg.mtype = STOP_MSG;
        msg.id = id;
        strcpy(msg.mtext, "STOP_MSG");

        if (msgsnd(msgid, &msg, MSGSZ, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("Sent STOP\n");
    }
    remove_client_queue_and_exit();
}

void send_list() {
    struct msgbuf msg;
    msg.mtype = LIST_MSG;
    msg.id = id;
    strcpy(msg.mtext, "LIST_MSG");

    if (msgsnd(msgid, &msg, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Sent LIST\n");
}

void send_2all(char* text) {
    struct msgbuf msg;
    msg.mtype = ALL_MSG;
    msg.id = id;
    strcpy(msg.mtext, text);

    if (msgsnd(msgid, &msg, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Sent 2ALL %s\n", text);
}

void send_2one(int receiver, char* text) {
    struct msgbuf msg;
    msg.mtype = ONE_MSG;
    msg.id = id;
    msg.receiver = receiver;
    strcpy(msg.mtext, text);

    if (msgsnd(msgid, &msg, MSGSZ, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Sent 2ONE %d %s\n", receiver, text);
}

void msg_receiver() {
    while(1) {
        struct msgbuf msg;

        if (msgrcv(queue_id, &msg, MSGSZ, -100, 0) == -1) {
            perror("msgrcv");
        }

        if(msg.mtype == STOP_MSG) {
            printf("Received STOP_MSG\n");
            kill(getppid() ,SIGINT);
            exit(0);
        }

        else if(msg.mtype == LIST_MSG) {
            printf("Received: %s\n", msg.mtext);
        }

        else if(msg.mtype == ALL_MSG) {
            printf("%s: Received ALL_MSG %s from %d\n", msg.tm, msg.mtext, msg.id);
        }

        else if(msg.mtype == ONE_MSG) {
            printf("%s: Received ONE_MSG %s from %d\n", msg.tm, msg.mtext, msg.id);
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
    }
}
