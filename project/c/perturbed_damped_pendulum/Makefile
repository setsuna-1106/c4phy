CC=gcc
CFLAGS = -Wall -Wextra -O2
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS = $(shell pkg-config --libs raylib)
TARGET=main

all:main

main:main.o rk4.o
	$(CC) $(CFLAGS) main.o rk4.o -o $(TARGET) $(RAYLIB_LIBS)

%.o:%.c
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -c $< -o $@

run:main
	./main

clean:
	rm -f *.o main

.PHONY:all main run clean
