CXXFLAGS = -std=c++20 `pkg-config opencv4 --cflags` -Wno-deprecated-anon-enum-enum-conversion
LDFLAGS = `pkg-config opencv4 --libs`
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
OUTPUT_NAME = space-sort

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT_NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

all: $(OUTPUT_NAME)

install: all
	cp $(OUTPUT_NAME) /usr/local/bin

clean:
	rm -f $(OUTPUT_NAME) $(OBJ_FILES)

.PHONY: all install clean
