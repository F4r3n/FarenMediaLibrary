
#include "Rendering/Texture.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
using namespace fm;
Texture::Texture(const std::string& path, Recti rect, bool alpha) : _path(path) {
    if(alpha) {
        _format = GL_RGBA;
    } else
        _format = GL_RGB;

    Image image;
    if(!image.loadImage(path)) {
        std::cout << "Error loading image " << path << std::endl;
    }

    // std::cout << "Texture " << texture.path << " Loaded " << texture.width <<
    // " " << texture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);  // All upcoming GL_TEXTURE_2D operations
                                       // now have effect on this texture object

    if(rect.w == -1 && rect.h == -1) {
        rect.w = image.getSize().x - rect.x;
        rect.h = image.getSize().y - rect.y;
    } else if(rect.x > image.getSize().x) {
        rect.x = 0;
    } else if(rect.y > image.getSize().y) {
        rect.y = 0;
    }
    image.getPart(_content, rect);
    _width = rect.w;
    _height = rect.h;
    // std::cout << rect.w << " " << rect.h << std::endl;
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 _format,
                 rect.w,
                 rect.h,
                 0,
                 _format,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
    image.clear();
    _content.clear();
    if(glGetError() != 0)
        std::cout << "Error Loading texture from path " << path << std::endl;
}

Texture::Texture(const Image& image, Recti rect) {
    // std::cout << "Texture " << texture.path << " Loaded " << texture.width <<
    // " " << texture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);  // All upcoming GL_TEXTURE_2D operations
                                       // now have effect on this texture object

    if(rect.w == -1 && rect.h == -1) {
        rect.w = image.getSize().x - rect.x;
        rect.h = image.getSize().y - rect.y;
    } else if(rect.x > image.getSize().x) {
        rect.x = 0;
    } else if(rect.y > image.getSize().y) {
        rect.y = 0;
    }
    image.getPart(_content, rect);
    _width = rect.w;
    _height = rect.h;

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    _content.clear();
}

void Texture::generate(int width, int height, Format format, Type type) {
    _width = width;
    _height = height;
    _type = type;
    _format = format;
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    if(format == Format::RGBA) {
        _numberChannels = 4;
    } else if(format == Format::RGB) {
        _numberChannels = 3;
    } else if(format == Format::RED) {
        _numberChannels = 1;
    }

    GLenum internalFormat;
    if(type == Type::FLOAT) {
        if(format == Format::RGBA)
            internalFormat = GL_RGBA32F;
        else if(format == Format::RGB)
            internalFormat = GL_RGB32F;
        else if(format == Format::RED)
            internalFormat = GL_R32F;
    } else if(type == Type::HALF_FLOAT) {
        if(format == Format::RGBA)
            internalFormat = GL_RGBA16F;
        else if(format == Format::RGB)
            internalFormat = GL_RGB16F;
        else if(format == Format::RED)
            internalFormat = GL_R16F;
    } else if(type == Type::UNSIGNED_BYTE) {
        internalFormat = format;
    }

    if(format == Format::DEPTH_STENCIL) {
        internalFormat = GL_DEPTH24_STENCIL8;
        if(type != Type::UNSIGNED_24_8) {
            type = Type::UNSIGNED_24_8;
        }
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internalFormat,
                 width,
                 height,
                 0,
                 format,
                 type,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::setData(unsigned char* image, bool alpha) {
    glBindTexture(GL_TEXTURE_2D, _id);
    if(!alpha)
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB16F,
                     _width,
                     _height,
                     0,
                     GL_RGB,
                     GL_FLOAT,
                     NULL);
    else
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     _width,
                     _height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_INT,
                     NULL);
}

Texture::Texture(std::vector<unsigned char>& data, Recti& rect, bool alpha) {
    init(data, rect);
}

void Texture::init(std::vector<unsigned char>& data, Recti& rect) {
    _width = rect.w;
    _height = rect.h;

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);  // All upcoming GL_TEXTURE_2D operations
                                       // now have effect on this texture object

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data.data());

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
}

void Texture::writeToPNG(const std::string& name) {
#if !OPENGL_ES
    if(_type == Type::UNSIGNED_BYTE) {
        std::cout << _width << " " << _height << " " << _numberChannels
                  << std::endl;
        unsigned char* data =
            new unsigned char[_width * _height * _numberChannels];
        glBindTexture(GL_TEXTURE_2D, _id);
        glGetTexImage(GL_TEXTURE_2D, 0, _format, _type, data);
        //        memset(data, 255, width*height*numberChannels);
        // for(int i = 0; i < width*numberChannels; i+=4) data[i] = 0;
        std::cout << (int)data[0] << std::endl;
        stbi_write_png(
            name.c_str(), _width, _height, 4, data, _width * _numberChannels);
        delete data;
    } else if(_type == Type::FLOAT) {
        float* data = new float[_width * _height * _numberChannels];
        glBindTexture(GL_TEXTURE_2D, _id);
        glGetTexImage(GL_TEXTURE_2D, 0, _format, _type, data);
        stbi_write_png(
            name.c_str(), _width, _height, 4, data, _width * _numberChannels);
        delete data;
    }
#endif
}

void Texture::setData(void* data, const fm::Recti& rect) {
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, rect.x, rect.y, rect.w, rect.h, _format, _type, data);
}

void Texture::setData(void* data) {
    glTexImage2D(
        GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, _type, data);
}

void Texture::setTo(int value, const fm::Recti& rect) {
    if(_type == Type::UNSIGNED_BYTE) {
        unsigned char* tempBuffer =
            new unsigned char[_width * _height * _numberChannels];
        memset(tempBuffer, value, _width * _height * _numberChannels);
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        _format,
                        _type,
                        tempBuffer);
        delete tempBuffer;
    } else if(_type == Type::FLOAT) {
        float* tempBuffer = new float[_width * _height * _numberChannels];
        memset(tempBuffer, value, _width * _height * _numberChannels);
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        _format,
                        _type,
                        tempBuffer);
        delete tempBuffer;
    }
}

Texture::Texture() {
}

void Texture::release() {
    // image.clear();
    glDeleteTextures(1, &_id);
}

void Texture::clear() {
    // image.clear();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

Texture::~Texture() {
}
