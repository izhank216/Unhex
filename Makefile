CC=gcc
CFLAGS=-O2
LDFLAGS=-lncurses -lm

all: bin/unhex

bin/unhex: src/unhex.c
	mkdir -p bin
	$(CC) $(CFLAGS) src/unhex.c -o bin/unhex $(LDFLAGS)

clean:
	rm -rf bin
