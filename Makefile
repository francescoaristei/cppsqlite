CXX := g++
CXXFLAGS := -std=c++17 -Icpp/include -g
LDFLAGS :=

# dirs
SRC_DIR := cpp/src
OBJ_DIR := obj
BIN_DIR := bin

# source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
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

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: run
run: $(MAIN)
	./$(MAIN)