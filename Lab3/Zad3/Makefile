CC = gcc
CFLAGS = -Wall
PROGRAM = main
DIRECTORY_PATH = ..
STRING = \#include

all:
	${CC} ${CFLAGS} ${PROGRAM}.c -o ${PROGRAM}
	./${PROGRAM} ${DIRECTORY_PATH} \${STRING}

clean:
	rm ${PROGRAM}
