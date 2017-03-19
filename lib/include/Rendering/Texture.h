#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"
#include "Core/Rect.h"
namespace fm {

enum Format {
    RED = GL_R,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    DEPTH_16 = GL_DEPTH_COMPONENT16,
    DEPTH_24 = GL_DEPTH_COMPONENT24,
    DEPTH_32 = GL_DEPTH_COMPONENT32,
    DEPTH_32F = GL_DEPTH_COMPONENT32F,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    STENCIL = GL_STENCIL_INDEX

};

enum Type {
    FLOAT = GL_FLOAT,
    HALF_FLOAT = GL_HALF_FLOAT,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    UNSIGNED_24_8 = GL_UNSIGNED_INT_24_8
};

enum Filter { NEAREST = GL_NEAREST, LINEAR = GL_LINEAR };

enum Wrapping {
    REPEAT = GL_REPEAT,
    CLAMP_BORDER = GL_CLAMP_TO_BORDER,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_EDGE = GL_CLAMP_TO_EDGE
};

class Texture {

public:
    Texture(const Image& image, Recti rect = { 0, 0, -1, -1 });
    Texture(const std::string& path, Recti rect = { 0, 0, -1, -1 }, bool alpha = true);
    Texture(std::vector<unsigned char>& data, Recti& rect, bool alpha = true);
    Texture(unsigned int width, unsigned int height);
    void setData(unsigned char* image, bool alpha);
    Texture();
    ~Texture();
    void bind() const;
    void clear();
    inline unsigned int getID() const {
        return id;
    }
    void generate(int width, int height, Format format, Type type);
    void release();
    Filter filter = NEAREST;
    Wrapping wrapping = REPEAT;

private:
    int width;
    int height;
    void init(std::vector<unsigned char>& data, Recti& rect);
    int format;
    std::string path;
    std::vector<unsigned char> content;

    GLuint id;
};
}
