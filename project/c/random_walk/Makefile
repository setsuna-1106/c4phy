# 两个可执行目标：
#   random_walk      随机行走本体（main.c）
#   protein_folding  蛋白质折叠自回避行走演示（protein_folding/main.c）

CC        ?= clang
BUILD_DIR := build
CFLAGS    := -Wall -Wextra -O2 -g
CFLAGS    += $(shell pkg-config --cflags raylib)
LDLIBS    := $(shell pkg-config --libs raylib) -lm

SRCS := main.c protein_folding/main.c
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# protein_folding 的对象在 build/protein_folding/ 子目录下，
# 二进制也放同名子目录，避免与目录本身路径冲突
PF_TARGET := $(BUILD_DIR)/protein_folding/protein_folding

all: $(BUILD_DIR)/random_walk $(PF_TARGET)

$(BUILD_DIR)/random_walk: $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(PF_TARGET): $(BUILD_DIR)/protein_folding/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -MMD -MP 自动生成头文件依赖（.d 文件），改头文件也会触发重编
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(BUILD_DIR)/random_walk
	./$(BUILD_DIR)/random_walk

run-protein-folding: $(PF_TARGET)
	./$(PF_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run run-protein-folding clean
