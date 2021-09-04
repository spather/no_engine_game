#include "shader_program.h"

#include <cstring>
#include <iostream>
#include <iterator>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::make_unique;
using std::string;
using std::stringstream;
using std::ostringstream;
using std::unique_ptr;

class ShaderProgramImpl: public ShaderProgram {
public:
  ShaderProgramImpl(GLuint id): id_(id) {}

  void use() const override;

private:
  GLuint id_;
};


tl::expected<string, string> loadFileContents(const char *filename) {
  ifstream file;
  file.open(filename);

  if (!file) {
    return tl::unexpected(strerror(errno));
  }

  string content((istreambuf_iterator<char>(file)),
    (istreambuf_iterator<char>()));

  return content;
}

tl::expected<GLuint, string> createAndCompileShader(
    GLenum shaderType, const char *source) {
  auto shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &source, NULL);
  glCompileShader(shaderID);

  int success;
  char infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);

    ostringstream message;
    message << "Error compiling "
      << (shaderType == GL_VERTEX_SHADER ? "vertex" :
          shaderType == GL_FRAGMENT_SHADER ? "fragment" : "unknown")
      << " shader:" << endl << infoLog;
    return tl::unexpected(message.str());
  }

  return shaderID;
}

tl::expected<unique_ptr<ShaderProgram>, ShaderProgramError> createShaderProgram(
    const char *vertexPath, const char *fragmentPath) {
  auto vSource = loadFileContents(vertexPath);
  if (!vSource) {
    ostringstream message;
    message << "Error opening vertex shader file \""
      << vertexPath << "\": " << vSource.error();
    return tl::unexpected(ShaderProgramError(message.str()));
  }

  auto fSource = loadFileContents(fragmentPath);
  if (!fSource) {
    ostringstream message;
    message << "Error opening fragment shader file \""
      << fragmentPath << "\": " << fSource.error();
    return tl::unexpected(ShaderProgramError(message.str()));
  }

  auto vertex = createAndCompileShader(GL_VERTEX_SHADER, (*vSource).c_str());
  auto frag = createAndCompileShader(GL_FRAGMENT_SHADER, (*fSource).c_str());

  if (!vertex) {
    return tl::unexpected(ShaderProgramError(vertex.error()));
  }

  if (!frag) {
    return tl::unexpected(ShaderProgramError(frag.error()));
  }

  auto id = glCreateProgram();
  glAttachShader(id, *vertex);
  glAttachShader(id, *frag);
  glLinkProgram(id);

  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    ostringstream message;
    message << "Error linking shader program: "
      << endl << infoLog;
    return tl::unexpected(ShaderProgramError(message.str()));
  }

  glDeleteShader(*vertex);
  glDeleteShader(*frag);

  return make_unique<ShaderProgramImpl>(id);
}

void ShaderProgramImpl::use() const {
  glUseProgram(id_);
}
