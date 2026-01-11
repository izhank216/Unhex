CC=gcc
CFLAGS=-O2
LDFLAGS=-lncurses
PY_CFLAGS=$(shell python3-config --includes)
PY_LDFLAGS=$(shell python3-config --ldflags)

all: bin/unhex python/Hex.so python/View.so

bin/unhex:
	mkdir -p bin
	$(CC) $(CFLAGS) src/unhex.c -o bin/unhex $(PY_CFLAGS) $(LDFLAGS) $(PY_LDFLAGS)

python/Hex.so:
	$(CC) -fPIC $(PY_CFLAGS) python/Hex.py -shared -o python/Hex.so $(PY_LDFLAGS)

python/View.so:
	$(CC) -fPIC $(PY_CFLAGS) python/View.py -shared -o python/View.so $(PY_LDFLAGS)

clean:
	rm -rf bin python/*.so
