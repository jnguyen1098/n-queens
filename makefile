CC = gcc
CFLAGS = -Wall -Ofast -Wextra -Wpedantic -ggdb
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY = run clean

run: queens
	./queens

queens: queens.o
	$(CC) $(CFLAGS) queens.o -o queens

queens.o: queens.c
	$(CC) $(CFLAGS) -c queens.c -o queens.o

clean:
	rm -rf queens queens.o
