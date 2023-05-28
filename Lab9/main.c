#include "santa.h"
#include "reindeer.h"
#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int main() {

    srand(time(NULL));

    pthread_cond_t santa_sleep_cond = PTHREAD_COND_INITIALIZER;

    int no_reindeers_waiting = 0;
    pthread_mutex_t no_reindeers_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t reindeers_sleep_cond = PTHREAD_COND_INITIALIZER;

    int no_elves_waiting = 0;
    int elves_waiting_ids[3] = {0};
    pthread_mutex_t no_elves_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t elves_sleep_cond = PTHREAD_COND_INITIALIZER;

    
    pthread_t santa;
    SantaData* santa_data = malloc(sizeof(SantaData));
    santa_data -> sleep_cond = &santa_sleep_cond;
    santa_data -> no_reindeers_waiting = &no_reindeers_waiting;
    santa_data -> no_reindeers_waiting_mutex = &no_reindeers_waiting_mutex;
    santa_data -> reindeers_sleep_cond = &reindeers_sleep_cond;
    santa_data -> no_elves_waiting = &no_elves_waiting;
    santa_data -> elves_waiting_ids = elves_waiting_ids;
    santa_data -> no_elves_waiting_mutex = &no_elves_waiting_mutex;
    santa_data -> elves_sleep_cond = &elves_sleep_cond;
    pthread_create(&santa, NULL, santa_thread, (void*) santa_data);


    pthread_t reindeers[NO_REINDEERS];
    ReindeerData* reindeers_data[NO_REINDEERS];

    for (int i=0; i<NO_REINDEERS; i++) {

        reindeers_data[i] = malloc(sizeof(ReindeerData));
        reindeers_data[i] -> ID = i;
        reindeers_data[i] -> no_reindeers_waiting = &no_reindeers_waiting;
        reindeers_data[i] -> no_reindeers_waiting_mutex = &no_reindeers_waiting_mutex;
        reindeers_data[i] -> reindeers_sleep_cond = &reindeers_sleep_cond;
        reindeers_data[i] -> santa_sleep_cond = &santa_sleep_cond;

        pthread_create(&reindeers[i], NULL, reindeer_thread, (void*) reindeers_data[i]);
    }


    pthread_t elves[NO_ELVES];
    ElfData* elves_data[NO_ELVES];

    for (int i=0; i<NO_ELVES; i++) {

        elves_data[i] = malloc(sizeof(ElfData));
        elves_data[i] -> ID = i;
        elves_data[i] -> no_elves_waiting = &no_elves_waiting;
        elves_data[i] -> elves_waiting_ids = elves_waiting_ids;
        elves_data[i] -> no_elves_waiting_mutex = &no_elves_waiting_mutex;
        elves_data[i] -> elves_sleep_cond = &elves_sleep_cond;
        elves_data[i] -> santa_sleep_cond = &santa_sleep_cond;

        pthread_create(&elves[i], NULL, elf_thread, (void*) elves_data[i]);
    }

    pthread_join(santa, NULL);


    free(santa_data);
    for (int i=0; i<NO_REINDEERS; i++) {
        pthread_cancel(reindeers[i]);
        free(reindeers_data[i]);
    }
    for (int i=0; i<NO_ELVES; i++) {
        pthread_cancel(elves[i]);
        free(elves_data[i]);
    }
    
    return 0;
}
