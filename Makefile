CXX = g++
CXXFLAGS = -std=c++17 -Wall -MMD -MP
BUILD_DIR = build

# 1. Find all .cpp files
SRCS = main.cpp $(shell find includes -name "*.cpp")

# 2. Transform SRCS into OBJS inside the build directory
# Example: includes/Board/Board.cpp -> build/includes/Board/Board.o
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# SFML and Include paths
INC_DIRS = -Iincludes -Iincludes/Piece -Iincludes/Board $(shell find includes/Pieces -type d | sed 's/^/-I/')
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = chess_game

all: $(TARGET)

# Linking stage
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Compilation stage
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

# Include the .d files for dependency tracking
-include $(OBJS:.o=.d)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
