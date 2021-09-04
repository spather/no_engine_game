cmake_minimum_required(VERSION 3.5)

project(third_party)

add_library(whereami_lib STATIC
  whereami/include/whereami.h
  whereami/src/whereami.c
)

target_include_directories(whereami_lib PUBLIC ./whereami/include)
