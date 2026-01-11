CC=gcc
CFLAGS=-O2
LDFLAGS=-lncurses -lm -ldl -rdynamic
PY_INC=-I/usr/include/python3.12
PY_LIB=-L/usr/lib/python3.12/config-3.12-x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lpython3.12

all: bin/unhex python/Hex.so python/View.so

bin/unhex:
	mkdir -p bin
	$(CC) $(CFLAGS) src/unhex.c -o bin/unhex $(PY_INC) $(PY_LIB) $(LDFLAGS)

python/Hex.c: python/Hex.py
	cython -3 -f -o python/Hex.c python/Hex.py

python/View.c: python/View.py
	cython -3 -f -o python/View.c python/View.py

python/Hex.so: python/Hex.c
	$(CC) -O2 -fPIC -shared python/Hex.c -o python/Hex.so $(PY_INC) $(PY_LIB)

python/View.so: python/View.c
	$(CC) -O2 -fPIC -shared python/View.c -o python/View.so $(PY_INC) $(PY_LIB)

clean:
	rm -rf bin python/*.so python/*.c python/__pycache__
