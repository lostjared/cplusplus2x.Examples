CXXFLAGS = -std=c++23
LDFLAGS =
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
OUTPUT_NAME =  wordsplit-shuffle-u
DEPS := $(wildcard *.hpp)

%.o: %.cpp $(DEPS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT_NAME): $(OBJ_FILES)
	$(CXX)  $^ -o $@ $(LDFLAGS)

all: $(OUTPUT_NAME)

install: all
	cp $(OUTPUT_NAME) /usr/local/bin

clean:
	rm -f $(OUTPUT_NAME) $(OBJ_FILES)

.PHONY: all install clean
