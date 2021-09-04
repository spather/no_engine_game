#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>

// TODO: put this in a namespace

class ShaderProgram {
public:
  ShaderProgram(const char *vertexPath, const char *fragmentPath);

  void use() const;

private:
  GLuint id_;
};

#endif
