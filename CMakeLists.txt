cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(no_engine_game)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/generated)
add_subdirectory(src/lib)
add_subdirectory(src/third-party)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/first)
add_subdirectory(src/app/first)
