# no_engine_game
This is an experiment in which I try to build a very simple game without a game engine. The goal is for me to learn fundamentals of low-level graphics programming.

## How to Build

1. Install [GLFW](https://www.glfw.org/).

On Mac, I did this with

```
brew install glfw
```

You can also download the source package from https://www.glfw.org/download.html and follow the [instructions to compile](https://www.glfw.org/docs/latest/compile_guide.html). You'll then have to install the built binaries and headers.

Installing in the default way should set things up so that CMake can find GLFW via `find_package()`. If you install some other way, you might have CMake trouble.

2. You should be able to then build as follows:

```
cd ~/code/no_engine_game # Or wherever it's located
mkdir -p build
cd build
cmake ..
cmake --build .
```

3. The source tree contains the files necessary to build and debug in VSCode. If you're using VSCode, you'll want to:
   * Install the [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools). You should also [enable C++ 17](https://stackoverflow.com/a/66251689).
   * Install the [CMake Extension](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
   * Install the [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
