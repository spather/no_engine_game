#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <whereami.h>

#include "shader_program.h"

namespace fs = std::filesystem;
using namespace std;

namespace no_engine_game { namespace app { namespace first {
void error_callback(int error, const char *description) {
  cerr << "ERROR: " << error << " " << description << endl;
}

void framebuffer_size_callback(
    __attribute__((unused)) GLFWwindow* _window, int width, int height) {
  glViewport(0, 0, width, height);
}

fs::path getCurrentPath() {
  int length = wai_getExecutablePath(NULL, 0, NULL);
  int dirnameLength = 0;

  char *path = new char[length+1];
  wai_getExecutablePath(path, length, &dirnameLength);
  path[dirnameLength] = '\0';

  return fs::path(path);
}
}}} // namespace no_engine_game::app:first

using namespace no_engine_game::lib;
using namespace no_engine_game::app::first;

int main() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    cerr << "GLFW Init failed" << endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(1024, 768, "GL Playground", NULL, NULL);
  if (!window) {
    cerr << "Couldn't create window" << endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cerr << "Error initializing GLAD" << endl;
    glfwTerminate();
    return 1;
  }

  GLsizei width;
  GLsizei height;

  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  unsigned int vao;
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0); // Unbind VAO

  auto path = getCurrentPath();

  auto shaderProgram = createShaderProgram(
    (path / "vertex_shader.vert").c_str(),
    (path / "fragment_shader.frag").c_str());

  if (shaderProgram) {
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      (*shaderProgram)->use();
      glBindVertexArray(vao);

      // Uncomment to draw in wireframe mode
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  }
  else {
    cerr << "Problem creating shader program: " << endl
      << shaderProgram.error().getMessage() << endl;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
