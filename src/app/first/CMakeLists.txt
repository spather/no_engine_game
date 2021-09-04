cmake_minimum_required(VERSION 3.5)

project(no_engine_game_app_first)

set(CMAKE_CXX_STANDARD 17)

add_executable(first main.cpp)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(first
  glad_lib
  no_engine_game_lib
  whereami_lib
  glfw
  OpenGL::GL
)

configure_file(fragment_shader.frag ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fragment_shader.frag COPYONLY)
configure_file(vertex_shader.vert ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vertex_shader.vert COPYONLY)
