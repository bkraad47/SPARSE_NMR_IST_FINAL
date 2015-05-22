

CC=gcc
CFLAGS=-I $(CFLAGS).

istmake: ist.i ist.c

	swig -python ist.i

	gcc -O3 -c ist.c ist_wrap.c $(CFLAGS) -lfftw3 -lpython2.7

	ld -shared ist.o ist_wrap.o -o _ist.so -lfftw3


