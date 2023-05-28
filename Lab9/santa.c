#include "santa.h"

void* santa_thread(void *arg) {

    SantaData* data = (SantaData*) arg;

    int no_gift_delivers = 0;

    while (no_gift_delivers < 3) {
        pthread_mutex_lock(data -> no_reindeers_waiting_mutex);
        pthread_cond_wait(data -> sleep_cond, data -> no_reindeers_waiting_mutex);
        pthread_mutex_lock(data -> no_elves_waiting_mutex);

        printf("Mikołaj: budzę się\n");

        if (*data -> no_elves_waiting == 3) {
            printf("Mikołaj: rozwiązuje problemy elfów ID %d %d %d\n", data -> elves_waiting_ids[0], data -> elves_waiting_ids[1], data -> elves_waiting_ids[2]);
            sleep(rand() % 2 + 1);
            *data -> no_elves_waiting = 0;
            pthread_cond_broadcast(data -> elves_sleep_cond);
        }

        if (*data -> no_reindeers_waiting == 9) {
            printf("Mikołaj: dostarczam zabawki\n");
            sleep(rand() % 3 + 2);
            no_gift_delivers++;
            *data -> no_reindeers_waiting = 0;
            pthread_cond_broadcast(data -> reindeers_sleep_cond);
        }

        pthread_mutex_unlock(data -> no_reindeers_waiting_mutex);
        pthread_mutex_unlock(data -> no_elves_waiting_mutex);

        printf("Mikołaj: zasypiam\n");
    }

    return NULL;
}
