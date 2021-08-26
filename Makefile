CC = clang
CPPC = clang++
CFLAGS = -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Iglad/include
CFLAGS += $(shell pkg-config --cflags glfw3)
CPPFLAGS = -std=c++17
LDFLAGS = $(shell pkg-config --static --libs glfw3) -stdlib=libc++

CPPSRC = src/main.cpp
CSRC = glad/src/glad.c
OBJ = $(CPPSRC:.cpp=.o) $(CSRC:.c=.o)
BIN = bin

all: dirs main

dirs:
	mkdir -p ./$(BIN)

main: $(OBJ)
	$(CPPC) -o $(BIN)/main $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CPPC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
