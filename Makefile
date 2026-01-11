CC=gcc
CFLAGS=-O2
LDFLAGS=-lncurses -lm -ldl
PY_CFLAGS=$(shell python3-config --includes)
PY_LDFLAGS=$(shell python3-config --ldflags)

all: bin/unhex python/Hex.so python/View.so

bin/unhex:
	mkdir -p bin
	$(CC) $(CFLAGS) src/unhex.c -o bin/unhex $(PY_CFLAGS) $(PY_LDFLAGS) $(LDFLAGS)

python/Hex.so:
	cython --embed -3 python/Hex.py -o python/Hex.so

python/View.so:
	cython --embed -3 python/View.py -o python/View.so

clean:
	rm -rf bin python/*.so
