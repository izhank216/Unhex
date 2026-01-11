CC=gcc
CFLAGS=-O2
LDFLAGS=-lncurses -lm -ldl -rdynamic
PY_INC=-I/usr/include/python3.12
PY_LIB=-L/usr/lib/python3.12/config-3.12-x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lpython3.12

all: bin/unhex python/Hex.so python/View.so

bin/unhex:
	mkdir -p bin
	$(CC) $(CFLAGS) src/unhex.c -o bin/unhex $(PY_INC) $(PY_LIB) $(LDFLAGS)

python/Hex.so:
	cython --embed -3 python/Hex.py -o python/Hex.so

python/View.so:
	cython --embed -3 python/View.py -o python/View.so

clean:
	rm -rf bin python/*.so
