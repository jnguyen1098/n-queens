CC = gcc
CFLAGS = -Wall -Ofast -Wextra -Wpedantic -ggdb -Iinclude
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
OBJECTS = queens.o
INCLUDE = queens.h

.PHONY = run clean main valgrind

run: main
	./main

valgrind: main
	valgrind $(VFLAGS) ./main

main: $(OBJECTS)
	$(CC) $(CFLAGS) main.c $(OBJECTS) -o main

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm *.o main
