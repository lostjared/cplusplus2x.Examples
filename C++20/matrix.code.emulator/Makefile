# Compiler and flags
#CXX = clang++
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
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)
