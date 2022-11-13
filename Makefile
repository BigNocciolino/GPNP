CC=gcc
# CFLAGS=-fPIC -Wall -Wextra -Werror -std=c11 -g
CFLAGS=-fPIC -Wall -Wextra -g
LIBS=-lmxml -lnmea
HEADERS=include/
SRC=src/decoder.c src/util.c src/gpx_creator.c
BLD=build

decoder: $(SRC)
	mkdir -p build
	mkdir -p out
	$(CC) $(CFLAGS) -I $(HEADERS) $(SRC) $(LIBS) -o $(BLD)/decoder

install:
	cp ./build/decoder /usr/local/bin/gpnp

clean:
	rm -rf build/ out/