CXX := g++
CXXFLAGS := -std=c++17 -Icpp/include -Icpp/include/datatypes  -Icpp/include/tools -g
LDFLAGS :=

# dirs
SRC_DIR := cpp/src
SRC_DATATYPES_DIR := $(SRC_DIR)/datatypes
SRC_TOOLS_DIR := $(SRC_DIR)/tools

TEST_DIR := cpp/tests

OBJ_DIR := obj
OBJ_TEST_DIR := obj_tests

BIN_DIR := bin
BIN_TEST_DIR := bin_tests


# source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DATATYPES_DIR)/*.cpp)  $(wildcard $(SRC_TOOLS_DIR)/*.cpp) 
TESTS := $(wildcard $(TEST_DIR)/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_TEST := $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(TESTS))

MAIN := $(BIN_DIR)/main
MAIN_TEST := $(BIN_TEST_DIR)/main_test

# targets
all: $(MAIN) $(MAIN_TEST)

$(MAIN): $(OBJ_DIR)/main.o $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DATATYPES_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_TOOLS_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(MAIN_TEST): $(OBJS_TEST) $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OBJ_TEST_DIR) $(BIN_TEST_DIR)

.PHONY: run
run: $(MAIN)
	./$(MAIN)

.PHONY: test
test: $(MAIN_TEST)
	./$(MAIN_TEST)
