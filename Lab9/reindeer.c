#include "reindeer.h"

void* reindeer_thread(void *arg) {

    ReindeerData* data = (ReindeerData*) arg;

    while (1) {
        sleep(rand() % 6 + 5);

        pthread_mutex_lock(data -> no_reindeers_waiting_mutex);

        (*data -> no_reindeers_waiting)++;
        printf("Renifer: czeka %d reniferów na Mikołaja, ID %d\n", *data -> no_reindeers_waiting, data -> ID);

        if (*data -> no_reindeers_waiting == NO_REINDEERS) {
                printf("Renifer: wybudzam Mikołaja, ID %d\n", data -> ID);
                pthread_cond_broadcast(data -> santa_sleep_cond);
                pthread_cond_wait(data -> reindeers_sleep_cond, data -> no_reindeers_waiting_mutex);
        }

        pthread_mutex_unlock(data -> no_reindeers_waiting_mutex);
    }

    return NULL;
}
