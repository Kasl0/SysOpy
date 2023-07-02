CC = gcc
CFLAGS = -Wall
SERVER = server
CLIENT = client
PORT = 7654

all:
	${CC} ${CFLAGS} ${SERVER}.c -o ${SERVER}
	${CC} ${CFLAGS} ${CLIENT}.c -o ${CLIENT}
	./${SERVER} ${PORT}
	make clean

clean:
	rm ${SERVER} ${CLIENT}
