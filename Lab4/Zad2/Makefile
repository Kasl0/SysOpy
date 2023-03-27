CC = gcc
CFLAGS = -Wall
SIGINFO_PROGRAM = siginfo
RESTART_PROGRAM = restart
NODEFER_PROGRAM = nodefer

all:
	make siginfo
	make restart
	make nodefer

siginfo:
	${CC} ${CFLAGS} ${SIGINFO_PROGRAM}.c -o ${SIGINFO_PROGRAM}

restart:
	${CC} ${CFLAGS} ${RESTART_PROGRAM}.c -o ${RESTART_PROGRAM}

nodefer:
	${CC} ${CFLAGS} ${NODEFER_PROGRAM}.c -o ${NODEFER_PROGRAM}

clean:
	rm ${SIGINFO_PROGRAM} ${RESTART_PROGRAM} ${NODEFER_PROGRAM}
