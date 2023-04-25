CC = gcc
CFLAGS = -Wall
SERVER = server
CLIENT = client
LOG = log.txt

all:
	${CC} ${CFLAGS} ${SERVER}.c -o ${SERVER}
	${CC} ${CFLAGS} ${CLIENT}.c -o ${CLIENT}
	./${SERVER}

clean:
	rm ${SERVER} ${CLIENT} ${LOG}
