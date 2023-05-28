#include "elf.h"

void* elf_thread(void *arg) {

    ElfData* data = (ElfData*) arg;

    while (1) {
        sleep(rand() % 4 + 2);

        pthread_mutex_lock(data -> no_elves_waiting_mutex);

        if (*data -> no_elves_waiting < 3) {
            
            data -> elves_waiting_ids[(*data -> no_elves_waiting)] = data -> ID;
            (*data -> no_elves_waiting)++;
            printf("Elf: czeka %d elfów na Mikołaja, ID %d\n", *data -> no_elves_waiting, data -> ID);

            if (*data -> no_elves_waiting == 3) {
                printf("Elf: wybudzam Mikołaja, ID %d\n", data -> ID);
                pthread_cond_broadcast(data -> santa_sleep_cond);
                pthread_cond_wait(data -> elves_sleep_cond, data -> no_elves_waiting_mutex);
                printf("Elf: Mikołaj rozwiązuje problem, ID %d\n", data -> ID);
            }

            else {
                pthread_cond_wait(data -> elves_sleep_cond, data -> no_elves_waiting_mutex);
                printf("Elf: Mikołaj rozwiązuje problem, ID %d\n", data -> ID);
            }
        }

        else {
            printf("Elf: samodzielnie rozwiązuję swój problem, ID %d\n", data -> ID);
        }
        
        pthread_mutex_unlock(data -> no_elves_waiting_mutex);
    }

    return NULL;
}
