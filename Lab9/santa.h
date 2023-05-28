#ifndef SANTA__H
#define SANTA__H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    pthread_cond_t *sleep_cond;

    int *no_reindeers_waiting;
    pthread_mutex_t *no_reindeers_waiting_mutex;
    pthread_cond_t *reindeers_sleep_cond;

    int *no_elves_waiting;
    int *elves_waiting_ids;
    pthread_mutex_t *no_elves_waiting_mutex;
    pthread_cond_t *elves_sleep_cond;
} SantaData;

void* santa_thread(void *arg);

#endif
