#include <nlohmann/json.hpp>
#include "Rendering/OpenGL/OGLTexture.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "Rendering/Image.h"

using namespace fm;


OGLTexture::OGLTexture(const std::string& path, Recti rect, bool alpha)
{
    if(alpha) 
	{
        _format = Format::RGBA;
    }
	else
        _format = Format::RGB;

    Image image;
    if(!image.loadImage(path)) 
	{
        std::cerr << "Error loading image " << path << std::endl;
    }

    // std::cout << "OGLTexture " << OGLTexture.path << " Loaded " << OGLTexture.width <<
    // " " << OGLTexture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture((GLenum)_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
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
    glTexImage2D((int)_textureKind,
                 0,
                 (int)_format,
                 rect.w,
                 rect.h,
                 0,
				(GLenum)_format,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture((GLenum)_textureKind, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
    image.clear();
    _content.clear();
    if(glGetError() != 0)
        std::cout << "Error Loading texture from path " << path << std::endl;
}

OGLTexture::OGLTexture(const OGLTexture&texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _content = std::vector<unsigned char>(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
}

OGLTexture& OGLTexture::operator=(const OGLTexture& texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _content = std::vector<unsigned char>(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
    return *this;
}

OGLTexture& OGLTexture::operator=(OGLTexture&& texture)
{
    _width = texture._width;
    _height = texture._height;
    _numberChannels = texture._numberChannels;
    _textureKind = texture._textureKind;
    _format = texture._format;
    _type = texture._type;
    _content.swap(texture._content);
    _id = texture._id;
    filter = texture.filter;
    wrapping = texture.wrapping;
    return *this;
}

OGLTexture::OGLTexture(const Image& image, Recti rect) {
    // std::cout << "Texture " << texture.path << " Loaded " << texture.width <<
    // " " << texture.height << std::endl;

    glGenTextures(1, &_id);
    glBindTexture((GLenum)_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
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

    glTexImage2D((GLenum)_textureKind,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 _content.data());

    glTexParameteri((GLenum)_textureKind,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_image_free(image);
    glBindTexture((GLenum)_textureKind, 0);
    _content.clear();
}

void OGLTexture::generate(size_t width, size_t height, Format format, Type type, int multiSampled) {
    _width = width;
    _height = height;
    _type = type;
    _format = format;
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
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
    glBindTexture((GLenum)_textureKind, _id);
    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
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
        internalFormat = (GLenum)format;
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
    }
    if(multiSampled <= 0)
    {
        glTexImage2D((GLenum)_textureKind,
                     0,
                     internalFormat,
                     (GLsizei)width,
                     (GLsizei)height,
                     0,
					(GLenum)format,
					(GLenum)type,
                     nullptr);
    }else
    {
        glTexImage2DMultisample((GLenum)_textureKind,
                     multiSampled,
                     internalFormat,
                     (GLsizei)width,
                     (GLsizei)height,
                     GL_TRUE
                       );
    }

    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
    }
    if(multiSampled <= 0)
    {
        glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_S, (GLint)wrapping);
        glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_T, (GLint)wrapping);
    }
    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
    }
    if(_textureKind == Kind::TEXTURE2D)
    {
        glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MIN_FILTER, (GLint)filter);
        glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MAG_FILTER, (GLint)filter);
    }


    glBindTexture((GLenum)_textureKind, 0);

    error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
    }
}

OGLTexture::OGLTexture(size_t width, size_t height) {
    _width = width;
    _height = height;
    glGenTextures(1, &_id);
    glBindTexture((GLenum)_textureKind, _id);

    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void OGLTexture::setData(unsigned char* image, bool alpha) {
    glBindTexture((GLenum)_textureKind, _id);
    if(!alpha)
        glTexImage2D((GLenum)_textureKind,
                     0,
                     GL_RGB16F,
                     (GLsizei)_width,
                     (GLsizei)_height,
                     0,
                     GL_RGB,
                     GL_FLOAT,
                     NULL);
    else
        glTexImage2D((GLenum)_textureKind,
                     0,
                     GL_RGBA,
                     (GLsizei)_width,
                     (GLsizei)_height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_INT,
                     NULL);
}

OGLTexture::OGLTexture(std::vector<unsigned char>& data, Recti& rect, bool alpha) {
    _init(data, rect);
}

void OGLTexture::_init(std::vector<unsigned char>& data, Recti& rect) {
    _width = rect.w;
    _height = rect.h;

    glGenTextures(1, &_id);
    glBindTexture((GLenum)_textureKind, _id);  // All upcoming GL_TEXTURE_2D operations
                                       // now have effect on this texture object

    glTexImage2D((GLenum)_textureKind,
                 0,
                 GL_RGBA,
                 rect.w,
                 rect.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data.data());

    glTexParameteri((GLenum)_textureKind,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri((GLenum)_textureKind, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture((GLenum)_textureKind, 0);  // Unbind texture when done, so we won't
                                      // accidentily mess up our texture.
}

void OGLTexture::GetPixel(const fm::math::vec2& inPosition, void *outValue) const
{
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture((GLenum)_textureKind, _id);
	glReadPixels((int)inPosition.x, (int)inPosition.y, 1, 1, (GLenum)_format, (GLenum)_type, outValue);
}

void OGLTexture::writeToPNG(const std::string& name) const
{

    if(_type == Type::UNSIGNED_BYTE)
    {
        unsigned char* data =  new unsigned char[_width * _height * _numberChannels];
        glBindTexture((GLenum)_textureKind, _id);
        glGetTexImage((GLenum)_textureKind, 0, (GLenum)_format, (GLenum)_type, data);

        stbi_write_png( name.c_str(), (int)_width, (int)_height, 4, data, (int)(_width * _numberChannels));
        delete [] data;
    }
    else if(_type == Type::FLOAT)
    {
        float* data = new float[_width * _height * _numberChannels];
        glBindTexture((GLenum)_textureKind, _id);
        glGetTexImage((GLenum)_textureKind, 0, (GLenum)_format, (GLenum)_type, data);
        stbi_write_png( name.c_str(), (GLsizei)_width, (GLsizei)_height, 4, data, (int)(_width * _numberChannels*sizeof(float)));
        delete [] data;
    }

}

void OGLTexture::setData(void* data, const fm::Recti& rect)
{
    glTexSubImage2D((GLenum)_textureKind, 0, rect.x, rect.y, rect.w, rect.h, (GLenum)_format, (GLenum)_type, data);
}

void OGLTexture::setData(void* data)
{
    glTexImage2D((GLenum)_textureKind, 0, (GLenum)_format, (GLsizei)_width, (GLsizei)_height, 0, (GLenum)_format, (GLenum)_type, data);
}

void OGLTexture::setTo(int value, const fm::Recti& rect)
{
    if(_type == Type::UNSIGNED_BYTE)
	{
        unsigned char* tempBuffer =
            new unsigned char[_width * _height * _numberChannels];
        memset(tempBuffer, value, _width * _height * _numberChannels);
        glTexSubImage2D((GLenum)_textureKind,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        (GLenum)_format,
                        (GLenum)_type,
                        tempBuffer);
        delete [] tempBuffer;
    } else if(_type == Type::FLOAT) {
        float* tempBuffer = new float[_width * _height * _numberChannels];
        memset(tempBuffer, value, _width * _height * _numberChannels);
        glTexSubImage2D((GLenum)_textureKind,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        (GLenum)_format,
                        (GLenum)_type,
                        tempBuffer);
        delete [] tempBuffer;
    }
}


void OGLTexture::release()
{
	if (_id > 0)
	{
		glDeleteTextures(1, &_id);
	}
}

void OGLTexture::clear()
{
}

void OGLTexture::bind() const {
    glBindTexture((GLenum)_textureKind, _id);
}

OGLTexture::~OGLTexture() {
}
