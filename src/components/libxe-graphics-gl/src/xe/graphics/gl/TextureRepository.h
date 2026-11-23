
#pragma once

#include <glaze/gl.h>
#include <map>
#include <string>

namespace xe {
    struct ImageData;
    class Renderer;
    class ImageLoader;
} // namespace xe
class TextureRepository {
public:
    explicit TextureRepository(xe::ImageLoader &loader);

    GLuint getOrCreate(const std::string &filePath, xe::Renderer &renderer);

    GLuint createTexture(xe::Renderer &renderer, const std::string &imageTypeStr, const size_t size, const void *imageData) const;

private:
    GLuint createTexture(const char *theFileName, xe::Renderer &renderer);

    GLuint createTexture(xe::Renderer &renderer, const xe::ImageData &image) const;

private:
    xe::ImageLoader &loader;
    std::map<std::string, GLuint> cachedTextureMap;
};
