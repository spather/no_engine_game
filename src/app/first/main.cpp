#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <whereami.h>

#include <shader_program.h>
#include <texture.h>

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
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0); // Unbind VAO

  auto path = getCurrentPath();

  auto shaderProgram = createShaderProgram(
    (path / "vertex_shader.vert").c_str(),
    (path / "fragment_shader.frag").c_str());

  auto texture0 = loadTexture(
      (path / "textures" / "container.jpg").c_str(),
      GL_RGB,
      GL_TEXTURE0);

  auto texture1 = loadTexture(
    (path / "textures" / "awesomeface.png").c_str(),
      GL_RGBA,
      GL_TEXTURE1);

  if (shaderProgram && texture0 && texture1) {
    (*shaderProgram)->use();
    (*shaderProgram)->assignTextureUnitToUniform("texture0", (*texture0).get());
    (*shaderProgram)->assignTextureUnitToUniform("texture1", (*texture1).get());

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      (*shaderProgram)->use();
      glBindVertexArray(vao);
      (*texture0)->bind();
      (*texture1)->bind();

      // Uncomment to draw in wireframe mode
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  }
  else {
    if (!shaderProgram) {
      cerr << "Problem creating shader program: " << endl
        << shaderProgram.error().getMessage() << endl;
    }
    if (!texture0) {
      cerr << "Problem creating texture: " << endl
        << texture0.error().getMessage() << endl;
    }
    if (!texture1) {
      cerr << "Problem creating texture: " << endl
        << texture1.error().getMessage() << endl;
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
