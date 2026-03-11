CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = main.c
OUT = tema1

.PHONY: all build run debug valgrind clean

all: build

build: $(OUT)

$(OUT): $(SRC) functii.h
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: build
	./$(OUT)

debug:
	$(CC) $(CFLAGS) -g -o $(OUT) $(SRC)

valgrind: build
	valgrind --leak-check=full ./$(OUT)

clean:
	rm -f $(OUT)
