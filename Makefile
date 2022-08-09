CC=gcc
CFLAGS=-Wall -Wextra -std=c11
LIBS=-lmxml -lnmea
HEADERS=include/
SRC=src/decoder.c src/util.c src/gpx_creator.c
BLD=build

decoder: $(SRC)
	mkdir -p build
	mkdir -p out
	$(CC) $(CFLAGS) -o $(BLD)/decoder -I $(HEADERS) $(SRC) -g $(LIBS)

clean:
	rm -rf build/ out/