#include <sstream>

#include <glad/glad.h>
#include <stb_image.h>

#include "include/texture.h"

using std::endl;
using std::make_unique;
using std::unique_ptr;
using std::ostringstream;

namespace no_engine_game { namespace lib {

namespace impl {

class TextureImpl: public Texture {
public:
  TextureImpl(GLuint id, GLenum unit): id_(id), unit_(unit) {}

  void bind() const override {
    glActiveTexture(unit_);
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  unsigned int getTextureUnitForUniform() const override {
    return unit_ - GL_TEXTURE0;
  }

private:
  GLuint id_;
  GLenum unit_;
};

class stbi_data_RAIIWrapper {
public:
  stbi_data_RAIIWrapper(stbi_uc *data): data_(data) {}
  ~stbi_data_RAIIWrapper() {
    stbi_image_free(data_);
  }

  stbi_uc *get() { return data_; }

private:
  stbi_uc *data_;
};

} // impl namespace

tl::expected<unique_ptr<Texture>, Error> loadTexture(
    const char *filename,
    GLenum pixelDataFormat,
    GLenum textureUnit) {

  // Should this be a parameter?
  stbi_set_flip_vertically_on_load(true);

  int txWidth, txHeight, nrChannels;
  auto data = impl::stbi_data_RAIIWrapper(stbi_load(
    filename,
    &txWidth,
    &txHeight,
    &nrChannels,
    0));

  if (data.get() == NULL) {
    ostringstream message;
    message << "Error loading image data from "
      << filename << endl
      << stbi_failure_reason() << endl;
    return tl::unexpected(Error(message.str()));
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // TODO: Probably a lot of this stuff needs to be paramaterized
  // but we can leave it hardcoded for now because I don't yet know
  // exactly what needs to be a param vs being hardcoded.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      txWidth,
      txHeight,
      0,
      pixelDataFormat,
      GL_UNSIGNED_BYTE,
      data.get());
  glGenerateMipmap(GL_TEXTURE_2D);
  return make_unique<impl::TextureImpl>(texture, textureUnit);
}

}}
