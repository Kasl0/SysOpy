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
	int start_cell;
	int stop_cell;
} ThreadData;

void signal_ignore(int signo) {
    ;
}

void* cell_thread(void *arg) {
	ThreadData* data = (ThreadData*) arg;
	signal(SIGUSR1, signal_ignore);

	while(true) {
		pause();
		for(int i = data->start_cell; i < data->stop_cell; i++) {
			(*(data->background))[i] = is_alive(i/get_grid_width(), i%get_grid_width(), *data->foreground);
		}
	}
	
	return NULL;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);

	int n = atoi(argv[1]);
	int no_cells_per_thread = get_grid_width()*get_grid_height()/n;
	pthread_t threads[n];

	for (int i = 0; i < n; i++) {

		ThreadData* data = malloc(sizeof(ThreadData));
		data->foreground = &foreground;
		data->background = &background;

		data->start_cell = i * no_cells_per_thread;
		data->stop_cell = (i+1) * no_cells_per_thread;
		if (i == n-1) data->stop_cell = get_grid_width()*get_grid_height();

		pthread_create(&threads[i], NULL, cell_thread, (void*) data);
	}

	while (true)
	{
		draw_grid(foreground);

		// Step simulation
		for (int i = 0; i < n; i++) pthread_kill(threads[i], SIGUSR1);
		usleep(500 * 1000);

		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
