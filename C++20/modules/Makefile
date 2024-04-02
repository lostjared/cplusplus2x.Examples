CXXFLAGS = -std=c++20 -fmodules-ts
LDFLAGS =
OBJ_FILES := mod.o main.o
OUTPUT_NAME = module-test
DEPS := $(wildcard *.hpp)

%.o: %.cpp $(DEPS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT_NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

all: $(OUTPUT_NAME)

install: all
	cp $(OUTPUT_NAME) /usr/local/bin

clean:
	rm -f $(OUTPUT_NAME) $(OBJ_FILES)

.PHONY: all install clean
