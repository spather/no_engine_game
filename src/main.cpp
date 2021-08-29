#include <fstream>
#include <iostream>
#include <iterator>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void error_callback(int error, const char *description) {
  cerr << "ERROR: " << error << " " << description << endl;
}

void framebuffer_size_callback(
    __attribute__((unused)) GLFWwindow* _window, int width, int height) {
  glViewport(0, 0, width, height);
}

string loadFromFile(string fileName) {
  ifstream ifs(fileName);

  // TODO: This should fail if the file can't be found
  string content((istreambuf_iterator<char>(ifs)),
    (istreambuf_iterator<char>()));

  return content;
}

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

  glViewport(0, 0, 1024, 768);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  unsigned int vao;
  glGenVertexArrays(1, &vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0); // Unbind VAO

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // TODO: make this find the file even if not loaded from the same directory
  // as the binary.
  const char *vertexShaderContent = loadFromFile("vertex_shader.vert").c_str();
  glShaderSource(vertexShader, 1, &vertexShaderContent, NULL);
  glCompileShader(vertexShader);
  // TODO: Get compile errors (see tutorial)

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // TODO: make this find the file even if not loaded from the same directory
  // as the binary.
  const char *fragmentShaderContent =
    loadFromFile("fragment_shader.frag").c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderContent, NULL);
  glCompileShader(fragmentShader);
  // TODO: Get compile errors (see tutorial)

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // TODO: Get link errors (see tutorial)

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
