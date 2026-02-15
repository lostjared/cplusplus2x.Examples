CXX = g++
CXXFLAGS = -std=c++23 -O2 -Wall
LDFLAGS =
LIBS = $(shell pkg-config --libs sdl3) -ldl -lGL
INCLUDES = -Iglad/include $(shell pkg-config --cflags sdl3)

SRC = src/main.cpp glad/src/glad.c
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

OUT = GL4_app

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) $(OBJ) -o $(OUT) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)

run: all
	./$(OUT)
