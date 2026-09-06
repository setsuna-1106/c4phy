CC        ?= clang
BUILD_DIR := build
CFLAGS    := -Wall -Wextra -O2 -g
CFLAGS    += -I../common
LDLIBS    := -lm

COMMON_DIR := ../common
vpath %.c $(COMMON_DIR)
vpath %.h $(COMMON_DIR)

SRCS   := main.c rk4_2d.c
OBJS   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/nucleon_in_box

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c $(COMMON_DIR)/rk4_2d.h
rk4_2d.o: rk4_2d.c $(COMMON_DIR)/rk4_2d.h

# -MMD -MP 自动生成头文件依赖（.d 文件），改头文件也会触发重编
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
