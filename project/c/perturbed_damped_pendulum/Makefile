CC=gcc
CFLAGS = -Wall -Wextra -O2
TARGET=main

all:main

main:main.o rk4.o
	$(CC) $(CFLAGS) main.o rk4.o -o $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

run:main
	./main

clean:
	rm -f *.o main

.PHONY:all main run clean