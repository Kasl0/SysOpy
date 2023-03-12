CC = gcc
CFLAGS = -Wall
PROGRAM = main

FROM = a
TO = f
INPUT = input.txt
OUTPUT = output.txt

all:
	${CC} ${CFLAGS} ${PROGRAM}.c -o ${PROGRAM}
	./${PROGRAM} ${FROM} ${TO} ${INPUT} ${OUTPUT}

clean:
	rm ${PROGRAM} ${OUTPUT}
