#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void error_callback(int error, const char *description) {
  cerr << "ERROR: " << error << " " << description << endl;
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cerr << "Error initializing GLAD" << endl;
    glfwTerminate();
    return 1;
  }

  glViewport(0, 0, 1024, 768);

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
