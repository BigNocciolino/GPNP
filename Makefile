CC=gcc
CFLAGS=-Wall -Wextra -std=c11
LIBS=-lmxml
HEADERS=include/
SRC=src/decoder.c src/nema_parser.c src/utils.c src/gpx_creator.c
BLD=build

decoder: $(SRC)
	mkdir -p build
	mkdir -p out
	$(CC) $(CFLAGS) -o $(BLD)/decoder -I $(HEADERS) $(SRC) $(LIBS)

clean:
	rm -rf build/ out/