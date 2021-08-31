CC = clang
CPPC = clang++
CFLAGS = -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Iglad/include -Isrc/third-party/whereami
CFLAGS += $(shell pkg-config --cflags glfw3)
CPPFLAGS = -std=c++17
LDFLAGS = $(shell pkg-config --static --libs glfw3) -stdlib=libc++

CPPSRC = $(shell find src/ -name "*.cpp")
CSRC = glad/src/glad.c $(shell find src/ -name "*.c")
OBJ = $(CPPSRC:.cpp=.o) $(CSRC:.c=.o)
BIN = bin

all: dirs main

dirs:
	mkdir -p ./$(BIN)

main: $(OBJ)
	$(CPPC) -o $(BIN)/main $^ $(LDFLAGS)
	cp src/*.frag bin/
	cp src/*.vert bin/

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CPPC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
