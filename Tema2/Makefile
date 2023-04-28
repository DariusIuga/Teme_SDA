CC = gcc
CFLAGS = -Wall -Wextra -g -O3

all: build

build: quadtree.c
	$(CC) $(CFLAGS) quadtree.c -o quadtree

run: build
	./quadtree

clean:
	rm -f quadtree quadtree.o