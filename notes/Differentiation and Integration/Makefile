CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LDLIBS  = -lm
TARGET  = diff

$(TARGET): main.o difference.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c difference.h
difference.o: difference.c difference.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: run clean
