CC = gcc
CFLAGS = -Wall -Wextra -O2

all: build

build: tema1.c list.h queue.h stack.h
	$(CC) $(CFLAGS) tema1.c -o tema1

run: build
	./tema1

clean:
	rm -f tema1 tema1.o list queue stack