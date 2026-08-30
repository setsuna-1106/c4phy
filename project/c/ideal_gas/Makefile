CC        ?= clang
BUILD_DIR := build
CFLAGS    := -Wall -Wextra -O2 -g
CFLAGS    += $(shell pkg-config --cflags raylib)
LDLIBS    := $(shell pkg-config --libs raylib) -lm

SRCS   := main.c
OBJS   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/ideal_gas

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

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
