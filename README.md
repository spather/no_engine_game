# no_engine_game
This is an experiment in which I try to build a very simple game without a game engine. The goal is for me to learn fundamentals of low-level graphics programming.

## How to Build

1. Initialize git submodules

After cloning the repo, run the following to get the submodules:

```
git submodule init
git submodule update
```

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
