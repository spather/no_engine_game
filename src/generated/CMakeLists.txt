cmake_minimum_required(VERSION 3.5)

project(generated)

add_library(glad_lib STATIC
  glad/include/glad/glad.h
  glad/include/KHR/khrplatform.h
  glad/src/glad.c
)

target_include_directories(glad_lib PUBLIC ./glad/include)
