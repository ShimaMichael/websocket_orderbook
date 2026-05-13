CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS := -MMD -MP
LDFLAGS :=
LIBS := -lboost_system -pthread -lssl -lcrypto -lboost_json

TARGET := streamer
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(shell find . -type f -name '*.cpp')
OBJS := $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean dirs

all: dirs $(BIN_DIR)/$(TARGET)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(sort $(dir $(OBJS)))

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)