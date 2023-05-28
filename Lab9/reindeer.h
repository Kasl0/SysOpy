#ifndef REINDEER__H
#define REINDEER__H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NO_REINDEERS 9

typedef struct {
	int ID;
	int *no_reindeers_waiting;
    pthread_mutex_t *no_reindeers_waiting_mutex;
    pthread_cond_t *reindeers_sleep_cond;
    pthread_cond_t *santa_sleep_cond;
} ReindeerData;

void* reindeer_thread(void *arg);

#endif
