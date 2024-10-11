# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++20 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf

# Target executable
TARGET = Matrix

# Source files
SRC = matrix.cpp

# Object files (replace .cpp with .o for each source file)
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)
