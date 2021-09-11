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
};

tl::expected<std::unique_ptr<Texture>, Error> loadTexture(
    const char *filename);

}}

#endif
