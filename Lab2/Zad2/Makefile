CC = gcc
CFLAGS = -Wall
PROGRAM = main

INPUT = input.txt
OUTPUT = output.txt

all:
	${CC} ${CFLAGS} ${PROGRAM}.c -o ${PROGRAM}
	./${PROGRAM} ${INPUT} ${OUTPUT}

clean:
	rm ${PROGRAM} ${OUTPUT}
