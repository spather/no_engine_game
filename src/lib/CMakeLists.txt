cmake_minimum_required(VERSION 3.5)

project(no_engine_game_lib)

add_library(no_engine_game_lib STATIC
  include/error.h
  include/shader_program.h
  include/texture.h
  shader_program.cpp
  stb_impl.cpp
  texture.cpp
)

find_package(tl-expected CONFIG REQUIRED)

target_include_directories(no_engine_game_lib PUBLIC ./include)
target_link_libraries(no_engine_game_lib PUBLIC glad_lib tl::expected)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(no_engine_game_lib PRIVATE ${STB_INCLUDE_DIRS})
