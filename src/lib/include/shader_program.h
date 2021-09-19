#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <memory>
#include <string>

#include <glad/glad.h>
#include <tl/expected.hpp>

#include "error.h"
#include "texture.h"

namespace no_engine_game { namespace lib {

class ShaderProgram {
public:
   virtual ~ShaderProgram() {};

   virtual void use() const = 0;

   // Eventually I'll add overloads of this for other data
   // types.
   virtual void setUniform(const char *name, GLint value) = 0;

   // Later, it might be necessary to expose parameters for
   // the number of matrices and whether to transpose.
   virtual void setUniformMatrix4fv(
      const char *name, const GLfloat *value) = 0;

   // Assigns the texture unit associated with a `Texture` to
   // a given uniform name. The value assigned will be the regular
   // integer (starting from 0), not the literal enum values
   // starting from GL_TEXTURE0. This is what the shaders expect.
   virtual void assignTextureUnitToUniform(
     const char *uniformName,
     const Texture *texture) = 0;
};

tl::expected<std::unique_ptr<ShaderProgram>, Error> createShaderProgram(
  const char *vertexPath, const char *fragmentPath
);

}}

#endif
