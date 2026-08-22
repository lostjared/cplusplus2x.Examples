CXXFLAGS = -std=c++20 `pkg-config opencv5 --cflags` 
LDFLAGS =  `pkg-config opencv5 --libs`
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
OUTPUT_NAME =  pattern-test
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
