# 两个可执行目标：
#   reflection_of_light_in_a_sphere  球内光线反射本体（Raylib 可视化）
#   circular_buffer_example          环形缓冲区独立小demo（仅 stdio，不链 Raylib）

CC        ?= clang
BUILD_DIR := build
CFLAGS    := -Wall -Wextra -O2 -g
CFLAGS    += $(shell pkg-config --cflags raylib)
LDLIBS    := $(shell pkg-config --libs raylib) -lm

SRCS := reflection_of_light_in_a_sphere.c circular_buffer_example.c
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(BUILD_DIR)/reflection_of_light_in_a_sphere $(BUILD_DIR)/circular_buffer_example

$(BUILD_DIR)/reflection_of_light_in_a_sphere: $(BUILD_DIR)/reflection_of_light_in_a_sphere.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/circular_buffer_example: LDLIBS :=
$(BUILD_DIR)/circular_buffer_example: $(BUILD_DIR)/circular_buffer_example.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -MMD -MP 自动生成头文件依赖（.d 文件），改头文件也会触发重编
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(BUILD_DIR)/reflection_of_light_in_a_sphere
	./$(BUILD_DIR)/reflection_of_light_in_a_sphere

run-circular-buffer-example: $(BUILD_DIR)/circular_buffer_example
	./$(BUILD_DIR)/circular_buffer_example

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run run-circular-buffer-example clean
