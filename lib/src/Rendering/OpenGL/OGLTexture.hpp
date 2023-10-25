#pragma once
#include <string>
#include <Core/Config.h>
#include "Core/Rect.h"
#include <nlohmann/json_fwd.hpp>
#include "GL/glew.h"

namespace fm {

	class Image;

enum class Format
{
    RED = GL_RED,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    DEPTH_16 = GL_DEPTH_COMPONENT16,
    DEPTH_24 = GL_DEPTH_COMPONENT24,
    DEPTH_32 = GL_DEPTH_COMPONENT32,
    DEPTH_32F = GL_DEPTH_COMPONENT32F,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    STENCIL = GL_STENCIL_INDEX
};

enum class Type
{
    FLOAT = GL_FLOAT,
    HALF_FLOAT = GL_HALF_FLOAT,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    UNSIGNED_24_8 = GL_UNSIGNED_INT_24_8
};

enum class Kind
{
    TEXTURE2D = GL_TEXTURE_2D,
    TEXTURE2D_MULTISAMPLED = GL_TEXTURE_2D_MULTISAMPLE
};

enum class Filter
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR
};

enum class Wrapping
{
    REPEAT = GL_REPEAT,
    CLAMP_BORDER = GL_CLAMP_TO_BORDER,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_EDGE = GL_CLAMP_TO_EDGE
};

class OGLTexture 
{
   public:
    OGLTexture(const Image& image, Recti rect = {0, 0, -1, -1});
    OGLTexture(const std::string& path,
            Recti rect = {0, 0, -1, -1},
            bool alpha = true);
    OGLTexture(std::vector<unsigned char>& data, Recti& rect, bool alpha = true);
    OGLTexture(size_t width, size_t height);
	OGLTexture() {};
	~OGLTexture();

	OGLTexture(const OGLTexture &OGLTexture);
	OGLTexture& operator=(const OGLTexture &OGLTexture);
	OGLTexture& operator=(OGLTexture &&OGLTexture);

    void				setData(unsigned char* image, bool alpha);

    void				bind() const;
    void				clear();
    inline GLuint		getID() const {return _id;}
    void				generate(size_t width, size_t height, Format format, Type type, int multiSampled = 0);
    void				release();


    void				writeToPNG(const std::string& name) const;
    void				setData(void* data);
    void				setTo(int value, const fm::Recti& rect);
    void				setData(void* data, const fm::Recti& rect);
    size_t				getWidth() const{ return _width; }
    size_t				getHeight() const{  return _height; }
    size_t				getNumberChannels() const{ return _numberChannels; }
	Kind				GetKind() const { return _textureKind; }
	void				GetPixel(const fm::math::vec2& inPosition, void *outValue) const;

	Filter filter = Filter::NEAREST;
	Wrapping wrapping = Wrapping::REPEAT;

   private:
	void				_init(std::vector<unsigned char>& data, Recti& rect);


    Kind _textureKind = Kind::TEXTURE2D;
    size_t _width = 0;
    size_t _height = 0;
    size_t _numberChannels = 4;

    Format _format = fm::Format::RGBA;
    Type _type = fm::Type::FLOAT;
    std::vector<unsigned char> _content;

    GLuint _id = 0;
};


}
