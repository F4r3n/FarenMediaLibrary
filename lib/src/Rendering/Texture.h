#pragma once
#include <string>
#include <Core/Config.h>
#include "Core/Rect.h"
#include <nlohmann/json_fwd.hpp>
namespace fm {

	class Image;

enum class Format
{
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

class Texture 
{
   public:
    Texture(const Image& image, Recti rect = {0, 0, -1, -1});
    Texture(const std::string& path,
            Recti rect = {0, 0, -1, -1},
            bool alpha = true);
    Texture(std::vector<unsigned char>& data, Recti& rect, bool alpha = true);
    Texture(unsigned int width, unsigned int height);
	Texture() {};
	~Texture();

	Texture(const Texture &texture);
	Texture& operator=(const Texture &texture);
	Texture& operator=(Texture &&texture);

    void				setData(unsigned char* image, bool alpha);

    void				bind() const;
    void				clear();
    inline unsigned int getID() const {return _id;}
    void				generate(int width, int height, Format format, Type type, int multiSampled = 0);
    void				release();


    void				writeToPNG(const std::string& name) const;
    void				setData(void* data);
    void				setTo(int value, const fm::Recti& rect);
    void				setData(void* data, const fm::Recti& rect);
    unsigned int		getWidth() const{ return _width; }
    unsigned int		getHeight() const{  return _height; }
    unsigned int		getNumberChannels() const{ return _numberChannels; }
	Kind				GetKind() const { return _textureKind; }
	void				GetPixel(const fm::math::vec2& inPosition, void *outValue) const;

	Filter filter = Filter::NEAREST;
	Wrapping wrapping = Wrapping::REPEAT;

   private:
	void				_init(std::vector<unsigned char>& data, Recti& rect);


    Kind _textureKind= Kind::TEXTURE2D;
    unsigned int _width;
    unsigned int _height;
    unsigned int _numberChannels = 4;

    Format _format;
    Type _type;
    std::vector<unsigned char> _content;

    GLuint _id;
};


}
