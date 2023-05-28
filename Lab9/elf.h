#ifndef ELF__H
#define ELF__H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NO_ELVES 10

typedef struct {
	int ID;
	int *no_elves_waiting;
    int *elves_waiting_ids;
    pthread_mutex_t *no_elves_waiting_mutex;
    pthread_cond_t *elves_sleep_cond;
    pthread_cond_t *santa_sleep_cond;
} ElfData;

void* elf_thread(void *arg);

#endif
