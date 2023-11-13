#pragma once
#include <string>
#include <Core/Config.h>
#include "Core/Rect.h"
#include <nlohmann/json_fwd.hpp>
#include "OGLTextureDef.hpp"

namespace fm {

	class Image;


class OGLTexture 
{
   public:
    OGLTexture(const Image& image, Recti rect);
	OGLTexture(const Image& image);

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
	void				bind(size_t inIndex) const;

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
	void				UploadImage(const Image& image);
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
