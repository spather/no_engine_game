#include "shader_program.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>

using std::ifstream;
using std::istreambuf_iterator;
using std::string;
using std::stringstream;

string loadFileContents(const char *filename) {
  ifstream file;
  file.exceptions(ifstream::failbit | ifstream::badbit);
  file.open(filename);

  string content((istreambuf_iterator<char>(file)),
    (istreambuf_iterator<char>()));

  return content;
}

GLuint createAndCompileShader(GLenum shaderType, const char *source) {
  auto shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &source, NULL);
  glCompileShader(shaderID);

  int success;
  char infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);

    std::cerr << "Shader compile error:  " << infoLog << std::endl;
    // TODO: should probably throw an exception here.
    return 0;
  }

  return shaderID;
}

ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath) {
  auto vSource = loadFileContents(vertexPath);
  auto fSource = loadFileContents(fragmentPath);

  auto vertex = createAndCompileShader(GL_VERTEX_SHADER, vSource.c_str());
  auto frag = createAndCompileShader(GL_FRAGMENT_SHADER, fSource.c_str());

  id_ = glCreateProgram();
  glAttachShader(id_, vertex);
  glAttachShader(id_, frag);
  glLinkProgram(id_);

  int success;
  char infoLog[512];
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, NULL, infoLog);
    std::cerr << "Shader program link error:  " << infoLog << std::endl;
    // TODO: should probably throw an exception here.
  }

  glDeleteShader(vertex);
  glDeleteShader(frag);
}

void ShaderProgram::use() const {
  glUseProgram(id_);
}
