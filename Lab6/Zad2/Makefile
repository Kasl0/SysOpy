CC = gcc
CFLAGS = -Wall
SERVER = server
CLIENT = client
LOG = log.txt

all:
	${CC} ${CFLAGS} ${SERVER}.c -o ${SERVER} -lrt
	${CC} ${CFLAGS} ${CLIENT}.c -o ${CLIENT} -lrt
	./${SERVER}

clean:
	rm ${SERVER} ${CLIENT} ${LOG}
