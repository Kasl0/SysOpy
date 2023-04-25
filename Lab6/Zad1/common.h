#define MAX_CLIENTS 10
#define MSG_MAX_LEN 256
#define SERVER_QUEUE_KEY 0x1234
#define MSGSZ sizeof(struct msgbuf) - sizeof(long)
#define STOP_MSG 1
#define LIST_MSG 2
#define INIT_MSG 3
#define ALL_MSG 4
#define ONE_MSG 5

struct msgbuf {
    long mtype;
    int id;
    int receiver;
    char mtext[MSG_MAX_LEN];
    char tm[MSG_MAX_LEN];
};
