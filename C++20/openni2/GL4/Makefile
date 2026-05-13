CXX = g++
CXXFLAGS = -std=c++20 `pkg-config sdl3 opencv4 --cflags` 

# Root path to the SDK you downloaded
SDK_ROOT = /home/jared/Downloads/OpenNI_2.3.0.86_202210111154_4c8f5aa4_beta6_linux

# Headers are located in sdk/include
INCLUDES = -I$(SDK_ROOT)/sdk/Include -I./include

# Shared libraries are typically in sdk/libs/x64
LIB_PATH = $(SDK_ROOT)/sdk/libs

# LDFLAGS Breakdown:
# -L: Tells the linker where to find libOpenNI2.so during build
# -lOpenNI2: Links the library
# -Wl,-rpath: Embeds the library path into the executable so it finds the 
#             Orbbec version at runtime instead of the system version.
LDFLAGS = -L$(LIB_PATH) $(LIB_PATH)/libOpenNI2.so -Wl,-rpath,$(LIB_PATH) `pkg-config sdl3 opencv4 --libs`

TARGET = test-astra
SRC = main.cpp src/cmx_gl.c src/glad.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
