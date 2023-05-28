CC = gcc
CFLAGS = -Wall
PROGRAM = main
SANTA = santa
REINDEER = reindeer
ELF = elf

all:
	${CC} ${CFLAGS} ${SANTA}.c ${REINDEER}.c ${ELF}.c ${PROGRAM}.c -o ${PROGRAM} -lpthread
	./${PROGRAM}
	make clean

clean:
	rm ${PROGRAM}
