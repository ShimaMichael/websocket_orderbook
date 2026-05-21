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

SRCS := $(shell find . -type f -name '*.cpp' -not -path './tests/*' -not -path './$(BUILD_DIR)/*' -not -path './$(BIN_DIR)/*')
TEST_SRCS := $(shell find tests -type f -name '*.cpp')

# Separate main.cpp so tests can link without the production `main()`
MAIN_SRC := $(wildcard ./main.cpp)
SRCS_NO_MAIN := $(filter-out ./main.cpp,$(SRCS))

OBJS := $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(SRCS_NO_MAIN))
MAIN_OBJ := $(if $(MAIN_SRC),$(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(MAIN_SRC)))
TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

.PHONY: all test clean dirs

all: dirs $(BIN_DIR)/$(TARGET)

test: dirs $(BIN_DIR)/$(TEST_TARGET)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(sort $(dir $(OBJS) $(TEST_OBJS)))

$(BIN_DIR)/$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(BIN_DIR)/$(TEST_TARGET): $(TEST_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(GTEST_CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS) $(GTEST_LIBS)

.PHONY: test
test: $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: run
run: $(BIN_DIR)/$(TARGET)
	@$(BIN_DIR)/$(TARGET)

-include $(DEPS)
-include $(TEST_DEPS)