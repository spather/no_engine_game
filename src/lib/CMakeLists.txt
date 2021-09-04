cmake_minimum_required(VERSION 3.5)

project(no_engine_game_lib)

add_library(no_engine_game_lib STATIC
  include/shader_program.h
  shader_program.cpp
)

target_include_directories(no_engine_game_lib PUBLIC ./include)
target_link_libraries(no_engine_game_lib glad_lib)
