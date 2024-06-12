CXX := g++
CXXFLAGS := -std=c++17 -Icpp/include -Icpp/include/datatypes  -Icpp/include/tools -g
LDFLAGS :=

# dirs
SRC_DIR := cpp/src
SRC_DATATYPES_DIR := $(SRC_DIR)/datatypes
SRC_TOOLS_DIR := $(SRC_DIR)/tools
OBJ_DIR := obj
BIN_DIR := bin

# source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DATATYPES_DIR)/*.cpp)  $(wildcard $(SRC_TOOLS_DIR)/*.cpp) 
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
MAIN := $(BIN_DIR)/main

# targets
all: $(MAIN)

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


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: run
run: $(MAIN)
	./$(MAIN)