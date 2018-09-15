#include <nlohmann/json.hpp>
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
        std::cerr << "Error loading image " << path << std::endl;
    }

    // std::cout << "Texture " << texture.path << " Loaded " << texture.width <<
    // " " << texture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture(_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
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
    glTexImage2D(_textureKind,
                 0,
                 _format,
                 rect.w,
                 rect.h,
                 0,
                 _format,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(_textureKind, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
    image.clear();
    _content.clear();
    if(glGetError() != 0)
        std::cout << "Error Loading texture from path " << path << std::endl;
}

Texture::Texture(const Texture &texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _path = texture._path;
    _content = std::vector<unsigned char>(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
}

Texture& Texture::operator=(const Texture &texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _path = texture._path;
    _content = std::vector<unsigned char>(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
    return *this;
}

Texture& Texture::operator=(Texture &&texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _path = texture._path;
    _content.swap(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
    return *this;
}

Texture::Texture(const Image& image, Recti rect) {
    // std::cout << "Texture " << texture.path << " Loaded " << texture.width <<
    // " " << texture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture(_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
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

    glTexImage2D(_textureKind,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri(_textureKind,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture(_textureKind, 0);
    _content.clear();
}

void Texture::generate(int width, int height, Format format, Type type, int multiSampled) {
    _width = width;
    _height = height;
    _type = type;
    _format = format;
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    glGenTextures(1, &_id);
    if(multiSampled > 0)
    {
        _textureKind = Kind::TEXTURE2D_MULTISAMPLED;
        wrapping = Wrapping::CLAMP_EDGE;
    }
    else
    {
        _textureKind = Kind::TEXTURE2D;
    }
    glBindTexture(_textureKind, _id);
    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }

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

    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    if(multiSampled <= 0)
    {
        glTexImage2D(_textureKind,
                     0,
                     internalFormat,
                     width,
                     height,
                     0,
                     format,
                     type,
                     nullptr);
    }else
    {
        glTexImage2DMultisample(_textureKind,
                     multiSampled,
                     internalFormat,
                     width,
                     height,
                     GL_TRUE
                       );
    }

    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    if(multiSampled <= 0)
    {
        glTexParameteri(_textureKind, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(_textureKind, GL_TEXTURE_WRAP_T, wrapping);
    }
    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    if(_textureKind == Kind::TEXTURE2D)
    {
        glTexParameteri(_textureKind, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(_textureKind, GL_TEXTURE_MAG_FILTER, filter);
    }


    glBindTexture(_textureKind, 0);

    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
}

Texture::Texture(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
    glGenTextures(1, &_id);
    glBindTexture(_textureKind, _id);

    glTexParameteri(_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::setData(unsigned char* image, bool alpha) {
    glBindTexture(_textureKind, _id);
    if(!alpha)
        glTexImage2D(_textureKind,
                     0,
                     GL_RGB16F,
                     _width,
                     _height,
                     0,
                     GL_RGB,
                     GL_FLOAT,
                     NULL);
    else
        glTexImage2D(_textureKind,
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
    glBindTexture(_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
                                       // now have effect on this texture object

    glTexImage2D(_textureKind,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data.data());

    glTexParameteri(_textureKind,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri(_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(_textureKind, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
}

void Texture::writeToPNG(const std::string& name) {
#if !OPENGL_ES
    if(_type == Type::UNSIGNED_BYTE)
    {
        unsigned char* data =  new unsigned char[_width * _height * _numberChannels];
        glBindTexture(_textureKind, _id);
        glGetTexImage(_textureKind, 0, _format, _type, data);

        stbi_write_png( name.c_str(), _width, _height, 4, data, _width * _numberChannels);
        delete data;
    }
    else if(_type == Type::FLOAT)
    {
        float* data = new float[_width * _height * _numberChannels];
        glBindTexture(_textureKind, _id);
        glGetTexImage(_textureKind, 0, _format, _type, data);
        stbi_write_png( name.c_str(), _width, _height, 4, data, _width * _numberChannels);
        delete data;
    }
#endif
}

void Texture::setData(void* data, const fm::Recti& rect) {
    glTexSubImage2D(
        _textureKind, 0, rect.x, rect.y, rect.w, rect.h, _format, _type, data);
}

void Texture::setData(void* data) {
    glTexImage2D(
        _textureKind, 0, _format, _width, _height, 0, _format, _type, data);
}

void Texture::setTo(int value, const fm::Recti& rect) {
    if(_type == Type::UNSIGNED_BYTE) {
        unsigned char* tempBuffer =
            new unsigned char[_width * _height * _numberChannels];
        memset(tempBuffer, value, _width * _height * _numberChannels);
        glTexSubImage2D(_textureKind,
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
        glTexSubImage2D(_textureKind,
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
    glBindTexture(_textureKind, _id);
}

Texture::~Texture() {
}
