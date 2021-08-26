# no_engine_game
This is an experiment in which I try to build a very simple game without a game engine. The goal is for me to learn fundamentals of low-level graphics programming.

## How to Build

1. Install [GLFW](https://www.glfw.org/).

I recommend downloading the source package from https://www.glfw.org/download.html and following the [instructions to compile](https://www.glfw.org/docs/latest/compile_guide.html).

For me, this ended up being:

```
cd ~/Downloads/glfw-3.3.4/ # Or wherever you downloaded the source package to
cmake -S . -B build
cd build
make
make install
```

This ends up installing the headers to `/usr/local/include/GLFW` and the lib to `/usr/local/lib`. If you use another method to install GLFW and the headers/lib end up somewhere else, you'll have to modify the `CFLAGS` and `LDFLAGS` in the Makefile to tell the compiler and linker where to find them.

2. You should be able to then build by just running `make` from the project root.
3. The source tree contains the files necessary to build and debug in VSCode. If you're using VSCode, you'll want to install the [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).
