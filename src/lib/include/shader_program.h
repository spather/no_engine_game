#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glad/glad.h>
#include <tl/expected.hpp>

// TODO: put this in a namespace

class ShaderProgram {
public:
   virtual ~ShaderProgram() {};

   virtual void use() const = 0;
};

class ShaderProgramError {
public:
  ShaderProgramError(std::string message): message_(message) {}

  std::string getMessage() { return message_; }

private:
  std::string message_;
};

tl::expected<std::unique_ptr<ShaderProgram>, ShaderProgramError> createShaderProgram(
  const char *vertexPath, const char *fragmentPath
);


#endif
