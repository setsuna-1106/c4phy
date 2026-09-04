CC=gcc
TARGET=main
COMMON_DIR=../common

vpath %.c $(COMMON_DIR)
vpath %.h $(COMMON_DIR)

INCLUDES := -I. -I$(COMMON_DIR)
CFLAGS= -Wall -Wextra -O2 $(INCLUDES)

SRCS = main.c rk4_2d.c
OBJS = main.o rk4_2d.o

all: $(TARGET)

# TODO(P2): 链接行应加 -lm (macOS 恰好不需要, Linux 上会因 exp/fabs 链接失败)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c $(COMMON_DIR)/rk4_2d.h
rk4_2d.o: rk4_2d.c $(COMMON_DIR)/rk4_2d.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all run clean
