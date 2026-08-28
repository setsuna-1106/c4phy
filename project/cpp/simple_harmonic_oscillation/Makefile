CXX       ?= clang++
BUILD_DIR := build
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -g
CXXFLAGS  += $(shell pkg-config --cflags sdl2)
LDLIBS    := $(shell pkg-config --libs sdl2)

SRCS   := main.cpp
OBJS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/simple_harmonic_oscillation

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
