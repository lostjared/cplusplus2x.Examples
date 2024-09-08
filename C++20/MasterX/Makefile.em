# Compiler
CXX = em++

# Compiler flags
CXXFLAGS = -std=c++20 -O2 -DFOR_WASM -s USE_SDL=2 -s USE_SDL_TTF=2 

# Output files
OUTPUT = MasterX.html

# Source files
SOURCES = src/dimension.cpp src/main.cpp src/splash.cpp src/window.cpp src/terminal.cpp

# Preload assets
PRELOAD = --preload-file assets

# Build target
all: $(OUTPUT)

# Rule to create the output (MasterX.html)
$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(OUTPUT)  $(PRELOAD) -s USE_SDL=2 -s USE_SDL_TTF=2 -s ALLOW_MEMORY_GROWTH -s ASSERTIONS

# Clean target
clean:
	rm -f $(OUTPUT)