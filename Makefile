CC=cc
CFLAGS=-Wall -Wextra -std=c11
LIBS=
HEADERS=include/
SRC=src/decoder.c src/nema_parser.c
BLD=build

decoder: $(SRC)
	mkdir -p build
# 	mkdir -p out
	$(CC) $(CFLAGS) -o $(BLD)/decoder -I $(HEADERS) $(SRC) $(LIBS)

clean:
	rm -rf build