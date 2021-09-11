#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glad/glad.h>
#include <tl/expected.hpp>

#include "error.h"

namespace no_engine_game { namespace lib {

class ShaderProgram {
public:
   virtual ~ShaderProgram() {};

   virtual void use() const = 0;
};

tl::expected<std::unique_ptr<ShaderProgram>, Error> createShaderProgram(
  const char *vertexPath, const char *fragmentPath
);

}}

#endif
