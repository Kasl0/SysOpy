CC = gcc
CFLAGS = -Wall
FORK_PROGRAM = fork
EXEC_PROGRAM = exec

COMMAND_IGNORE = ignore
COMMAND_HANDLER = handler
COMMAND_MASK = mask
COMMAND_PENDING = pending

all:
	make fork
	make exec

fork:
	${CC} ${CFLAGS} ${FORK_PROGRAM}.c -o ${FORK_PROGRAM}
	./${FORK_PROGRAM} ${COMMAND_IGNORE}

exec:
	${CC} ${CFLAGS} ${EXEC_PROGRAM}.c -o ${EXEC_PROGRAM}
	./${EXEC_PROGRAM} ${COMMAND_IGNORE}

clean:
	rm ${FORK_PROGRAM} ${EXEC_PROGRAM}
