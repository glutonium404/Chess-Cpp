CXX = g++
# -Wall: show all warnings | -std=c++17: use modern C++
CXXFLAGS = -Wall -std=c++17

# Search for all .cpp files recursively
SRCS = main.cpp $(shell find includes -name "*.cpp")

# List all directories containing headers for the -I flag
INC_DIRS = -Iincludes/Board -Iincludes/Piece $(shell find includes/Pieces -type d | sed 's/^/-I/')

# SFML Libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Target name
TARGET = chess_game

all:
	$(CXX) $(CXXFLAGS) $(SRCS) $(INC_DIRS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
