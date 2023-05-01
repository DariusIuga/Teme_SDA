CC = gcc
CFLAGS = -Wall -Wextra -g -O3

all: build

build: quadtree.c tree_operations.h cerinta1.h cerinta2.h cerinta3.h 
	$(CC) $(CFLAGS) quadtree.c -o quadtree

run: build
	./quadtree

clean:
	rm -f quadtree quadtree.o tree_operations cerinta1 cerinta2 cerinta3