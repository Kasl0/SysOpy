CC = gcc
CFLAGS = -Wall
SENDER_PROGRAM = sender
CATCHER_PROGRAM = catcher

all:
	make sender
	make catcher
	./catcher

sender:
	${CC} ${CFLAGS} ${SENDER_PROGRAM}.c -o ${SENDER_PROGRAM}

catcher:
	${CC} ${CFLAGS} ${CATCHER_PROGRAM}.c -o ${CATCHER_PROGRAM}

clean:
	rm ${SENDER_PROGRAM} ${CATCHER_PROGRAM}
