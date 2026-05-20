CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS := -MMD -MP
LDFLAGS :=
LIBS := -lboost_system -pthread -lssl -lcrypto -lboost_json
GTEST_PREFIX := $(shell brew --prefix googletest)
GTEST_CXXFLAGS := -isystem $(GTEST_PREFIX)/include
GTEST_LIBS := -L$(GTEST_PREFIX)/lib -lgtest -lgtest_main

TARGET := streamer
TEST_TARGET := unit_tests
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(shell find . -type f -name '*.cpp' -not -path './tests/*')
TEST_SRCS := $(shell find tests -type f -name '*.cpp')
OBJS := $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

.PHONY: all test clean dirs

all: dirs $(BIN_DIR)/$(TARGET)

test: dirs $(BIN_DIR)/$(TEST_TARGET)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(sort $(dir $(OBJS) $(TEST_OBJS)))

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(BIN_DIR)/$(TEST_TARGET): $(TEST_OBJS) $(BUILD_DIR)/src/orderbook/orderbook.o $(BUILD_DIR)/src/Utilities/helper.o
	$(CXX) $(CXXFLAGS) $(GTEST_CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS) $(GTEST_LIBS)

.PHONY: test
test: $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GTEST_CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)
-include $(TEST_DEPS)