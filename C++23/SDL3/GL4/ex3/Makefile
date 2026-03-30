
CXX=g++
CXXFLAGS=-std=c++23 -O2
LIBS=`pkg-config --libs sdl3` -lGL -ldl
INCLUDES=`pkg-config --cflags sdl3` -Iglad/include
OUTPUT=FractalApp
all:
	$(CXX) src/main.cpp glad/src/glad.cpp -o $(OUTPUT) $(CXXFLAGS) $(INCLUDES) $(LIBS)

clean:
	rm -f $(OUTPUT)
