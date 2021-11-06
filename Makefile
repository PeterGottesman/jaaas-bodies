SOURCE_DIRS=$(shell find ./src/ -type d)
BUILD_DIRS=$(SOURCE_DIRS:./src/%=./build/%)

SOURCES=$(shell find ./src/ -type f -iname "*.cpp")
INCLUDES=$(shell find ./inc/ -type f -iname "*.h")
OBJS=$(SOURCES:./src/%.cpp=build/%.o)
EXEC=jaaas

LIBS=glfw3 glew 

CXX=g++
CFLAGS=-ggdb -Ofast $(pkg-config --cflags $(LIBS)) -Wall
CFLAGS+=-I./inc -march=native
LDFLAGS=`pkg-config --libs $(LIBS)` -lpthread

default: $(EXEC)

$(EXEC): $(BUILD_DIRS) $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIRS):
	[ -d "$@" ] || mkdir $@

build/%.o: src/%.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build $(EXEC)

.PHONY: default clean
