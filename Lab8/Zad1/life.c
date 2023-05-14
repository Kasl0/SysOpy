#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

typedef struct {
	char **foreground;
	char **background;
	int row;
	int col;
} ThreadData;

void signal_ignore(int signo) {
    ;
}

void* cell_thread(void *arg) {
	ThreadData* data = (ThreadData*) arg;
	signal(SIGUSR1, signal_ignore);

	while(true) {
		pause();
		(*(data->background))[data->row * get_grid_width() + data->col] = is_alive(data->row, data->col, *data->foreground);
	}
	
	return NULL;
}

int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;


	init_grid(foreground);

	pthread_t threads[get_grid_height()][get_grid_width()];

	for (int i = 0; i < get_grid_height(); ++i) {
        for (int j = 0; j < get_grid_width(); ++j) {

			ThreadData* data = malloc(sizeof(ThreadData));
			data->foreground = &foreground;
			data->background = &background;
        	data->row = i;
        	data->col = j;
			pthread_create(&threads[i][j], NULL, cell_thread, (void*) data);

		}
	}

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
		for (int i = 0; i < get_grid_height(); ++i) {
			for (int j = 0; j < get_grid_width(); ++j) {

				pthread_kill(threads[i][j], SIGUSR1);
			}
		}
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
