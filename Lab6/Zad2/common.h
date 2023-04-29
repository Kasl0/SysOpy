#define MAX_CLIENTS 10
#define MSG_MAX_LEN 256
#define SERVER_QUEUE_NAME "/server_queue"
#define MSGSZ sizeof(struct msgbuf)
#define STOP_MSG 1
#define LIST_MSG 2
#define INIT_MSG 3
#define ALL_MSG 4
#define ONE_MSG 5

struct msgbuf {
    int id;
    int receiver;
    char mtext[MSG_MAX_LEN];
    char tm[MSG_MAX_LEN];
};
