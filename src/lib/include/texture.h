#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>

#include <tl/expected.hpp>

#include "error.h"

namespace no_engine_game { namespace lib {

class Texture {
public:
  virtual ~Texture() {}

  virtual void bind() const = 0;

  // Returns the texture unit as a regular number,
  // starting from 0. So if the texture unit is
  // GL_TEXTURE0, this will return 0, if GL_TEXTURE1,
  // then 1, etc.
  virtual unsigned int getTextureUnitForUniform() const = 0;
};

tl::expected<std::unique_ptr<Texture>, Error> loadTexture(
    const char *filename,
    GLenum pixelDataFormat,
    GLenum textureUnit);

}}

#endif
