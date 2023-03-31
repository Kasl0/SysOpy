CC = gcc
CFLAGS = -Wall
PROGRAM = main
WIDTH = 0.1
NO_PROGRAMS = 4
EXEC = exec

all:
	make main
	make exec
	./${PROGRAM} ${WIDTH} ${NO_PROGRAMS}

main:
	${CC} ${CFLAGS} ${PROGRAM}.c -o ${PROGRAM}

exec:
	${CC} ${CFLAGS} ${EXEC}.c -o ${EXEC}

clean:
	rm ${PROGRAM} ${EXEC} potok
