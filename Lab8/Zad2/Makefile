CC = gcc
CFLAGS = -Wall
NO_THREADS = 3

all: life
	
life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncursesw -lpthread
	./life $(NO_THREADS)
	
clean:
	rm life
