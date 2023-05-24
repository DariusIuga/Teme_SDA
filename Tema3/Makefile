CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Wpedantic -g -O3

all: build

build: tema3.c task1.h task2.h
	$(CC) $(CFLAGS) tema3.c -o tema3

run: build
	./tema3

clean:
	rm -f tema3 tema3.o task1 task2