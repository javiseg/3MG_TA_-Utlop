#pragma once
#include "glad/glad.h"
#include <string>
#include <vector>

namespace Utlop {

  struct Texture {
    GLuint id;
    const char* type;
    GLuint unit;
    Texture(const char* path, const char* texType, GLenum slot, GLenum format, GLenum pixelType);

    void texUnit(GLuint shader, const char* uniform, GLuint unit);
    void bind();
    void unbind();
    void deleteTexture();
  };

  struct Material {
    std::vector<Texture> textures;
  };


}