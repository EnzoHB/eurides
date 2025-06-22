CC = gcc
CFLAGS = -I./include -std=c99
SRC = $(shell find src -name '*.c')
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

run: all
	./$(OUT)