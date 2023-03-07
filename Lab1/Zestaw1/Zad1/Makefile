CC = gcc
CFLAGS = -Wall
LIB = libwc

all:
	make lib_static
	make lib_shared

lib_static:
	${CC} ${CFLAGS} -c $(LIB).c
	ar rcs $(LIB).a $(LIB).o

lib_shared:
	${CC} ${CFLAGS} -fPIC -c $(LIB).c
	${CC} ${CFLAGS} -shared $(LIB).o -o $(LIB).so
	sudo cp $(LIB).so /usr/lib/$(LIB).so
	sudo ldconfig

clean:
	rm -f *.a *.o *.so