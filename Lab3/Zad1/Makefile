CC = gcc
CFLAGS = -Wall
PROGRAM = main
NO_PROCESSES = 10

all:
	${CC} ${CFLAGS} ${PROGRAM}.c -o ${PROGRAM}
	./${PROGRAM} ${NO_PROCESSES}

clean:
	rm ${PROGRAM}
