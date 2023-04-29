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

int id;
mqd_t server_queue;
mqd_t client_queue;
char* client_queue_name;

void open_server_queue();
char* generate_random_string();
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

    mq_unlink("/client_queue");

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

    server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_queue == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    printf("Server queue opened\n");
}

char* generate_random_string() {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    static const int alphabet_size = sizeof(alphabet) - 1;

    char* result = malloc(10);

    result[0] = '/';

    srand(time(NULL));

    for (int i = 1; i < 9; i++) {
        int index = rand() % alphabet_size;
        result[i] = alphabet[index];
    }

    result[9] = '\0';

    return result;
}

void create_client_queue() {

    client_queue_name = generate_random_string();
    printf("Client queue name: %s\n", client_queue_name);

    struct mq_attr atr;
	atr.mq_flags = 0;
	atr.mq_maxmsg = MAX_CLIENTS;
	atr.mq_msgsize = MSGSZ;
	atr.mq_curmsgs = 0;
    
    client_queue = mq_open(client_queue_name, O_CREAT | O_RDONLY, 0666, &atr);
    if (client_queue == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    printf("Client queue created\n");
}

void remove_client_queue_and_exit() {
    if (server_queue > 0) {
        if (mq_close(server_queue) == -1) {
            perror("mq_close");
        }
        printf("Closed server queue\n");
        server_queue = 0;
    }
    if (client_queue > 0) {
        if (mq_close(client_queue) == -1) {
            perror("mq_close");
        }
        mq_unlink(client_queue_name);
        printf("Closed and removed client queue\n");
        client_queue = 0;
    }
    exit(0);
}

void send_init() {
    struct msgbuf msg;
    msg.id = -1;
    strcpy(msg.mtext, client_queue_name);

    if (mq_send(server_queue, (const char *)&msg, MSGSZ, INIT_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("Init message sent\n");
}

void receive_id() {
    struct msgbuf msg;
    if (mq_receive(client_queue, (char*)&msg, MSGSZ, NULL) == -1) {
        perror("mq_receive");
    }
    
    id = atoi(msg.mtext);
    printf("Received ID: %d\n", id);
}

void send_stop() {
    if (server_queue > 0) {
        struct msgbuf msg;
        msg.id = id;
        strcpy(msg.mtext, "STOP_MSG");

        if (mq_send(server_queue, (const char *)&msg, MSGSZ, STOP_MSG) == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
        printf("Sent STOP\n");
    }
    remove_client_queue_and_exit();
}

void send_list() {
    struct msgbuf msg;
    msg.id = id;
    strcpy(msg.mtext, "LIST_MSG");

    if (mq_send(server_queue, (const char *)&msg, MSGSZ, LIST_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("Sent LIST\n");
}

void send_2all(char* text) {
    struct msgbuf msg;
    msg.id = id;
    strcpy(msg.mtext, text);

    if (mq_send(server_queue, (const char *)&msg, MSGSZ, ALL_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("Sent 2ALL %s\n", text);
}

void send_2one(int receiver, char* text) {
    struct msgbuf msg;
    msg.id = id;
    msg.receiver = receiver;
    strcpy(msg.mtext, text);

    if (mq_send(server_queue, (const char *)&msg, MSGSZ, ONE_MSG) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("Sent 2ONE %d %s\n", receiver, text);
}

void msg_receiver() {
    while(1) {
        struct msgbuf msg;
        unsigned int type;

        if (mq_receive(client_queue, (char*)&msg, MSGSZ, &type) == -1) {
            perror("mq_receive");
        }

        if(type == STOP_MSG) {
            printf("Received STOP_MSG\n");
            kill(getppid() ,SIGINT);
            exit(0);
        }

        else if(type == LIST_MSG) {
            printf("Received: %s\n", msg.mtext);
        }

        else if(type == ALL_MSG) {
            printf("%s: Received ALL_MSG %s from %d\n", msg.tm, msg.mtext, msg.id);
        }

        else if(type == ONE_MSG) {
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
