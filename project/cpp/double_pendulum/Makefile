CXX       ?= clang++
BUILD_DIR := build
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -g -Iinclude
CXXFLAGS  += $(shell pkg-config --cflags sdl2 SDL2_ttf)
LDLIBS    := $(shell pkg-config --libs sdl2 SDL2_ttf)

SRCS   := main.cpp $(wildcard src/*/*.cpp)
OBJS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/double_pendulum

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# -MMD -MP 自动生成头文件依赖（.d 文件），改 include/ 下的头文件也会触发重编
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
