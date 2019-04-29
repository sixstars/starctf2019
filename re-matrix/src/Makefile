#
# Makefile
# hzshang, 2019-03-29 16:24
#
CFLAGS=-O3
all:
	gcc mutate.c main.c $(CFLAGS) -o main
	strip main
32:
	./tcc32 -L /lib32 -I ./tcc/i686-linux-gnu/include main.c -c -g
	./tcc32 -L /lib32 -I ./tcc/i686-linux-gnu/include mutate.c -c -g
	i686-linux-gnu-gcc-7 -L./tcc/i686-linux-gnu/lib/tcc main.o mutate.o ./tcc/i686-linux-gnu/lib/tcc/libtcc1.a -o ./cube
	strip cube

debug:
	gcc mutate.c main.c $(CFLAGS) -o main -D DEBUG
	strip main


# vim:ft=make
#
