CC = gcc
CFLAGS = -Wall
PROGRAM = main
LIB = ../Zad1/libwc

all:
	make lib_static_O0
	make lib_static_Os
	make lib_static_O3

	make lib_shared_O0
	make lib_shared_Os
	make lib_shared_O3

	make lib_dynamic_O0
	make lib_dynamic_Os
	make lib_dynamic_O3

lib_static_O0:
	${CC} ${CFLAGS} -O0 -c ${PROGRAM}.c -o ${PROGRAM}_O0.o
	${CC} ${CFLAGS} -O0 ${PROGRAM}_O0.o $(LIB).a -o ${PROGRAM}_static_O0
lib_static_Os:
	${CC} ${CFLAGS} -Os -c ${PROGRAM}.c -o ${PROGRAM}_Os.o
	${CC} ${CFLAGS} -Os ${PROGRAM}_Os.o $(LIB).a -o ${PROGRAM}_static_Os
lib_static_O3:
	${CC} ${CFLAGS} -O3 -c ${PROGRAM}.c -o ${PROGRAM}_O3.o
	${CC} ${CFLAGS} -O3 ${PROGRAM}_O3.o $(LIB).a -o ${PROGRAM}_static_O3

lib_shared_O0:
	${CC} ${CFLAGS} -O0 -c ${PROGRAM}.c -o ${PROGRAM}_O0.o
	${CC} ${CFLAGS} -O0 ${PROGRAM}_O0.o -L. -lwc -Wl,-rpath=. -o ${PROGRAM}_shared_O0
lib_shared_Os:
	${CC} ${CFLAGS} -Os -c ${PROGRAM}.c -o ${PROGRAM}_Os.o
	${CC} ${CFLAGS} -Os ${PROGRAM}_Os.o -L. -lwc -Wl,-rpath=. -o ${PROGRAM}_shared_Os
lib_shared_O3:
	${CC} ${CFLAGS} -O3 -c ${PROGRAM}.c -o ${PROGRAM}_O3.o
	${CC} ${CFLAGS} -O3 ${PROGRAM}_O3.o -L. -lwc -Wl,-rpath=. -o ${PROGRAM}_shared_O3

lib_dynamic_O0:
	${CC} ${CFLAGS} -O0 ${PROGRAM}_dyn.c -o ${PROGRAM}_dynamic_O0 -ldl
lib_dynamic_Os:
	${CC} ${CFLAGS} -Os ${PROGRAM}_dyn.c -o ${PROGRAM}_dynamic_Os -ldl
lib_dynamic_O3:
	${CC} ${CFLAGS} -O3 ${PROGRAM}_dyn.c -o ${PROGRAM}_dynamic_O3 -ldl

clean:
	rm -f *.a *.o *.so