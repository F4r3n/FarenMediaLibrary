#include "Rendering/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>
#include <nlohmann/json.hpp>
using namespace fm;
Image::Image(const fm::FilePath& inPath)
{
	_path = inPath;
}

Image::~Image()
{
	clear();
}


void Image::clear()
{
	_size.reset();
	if (_pixel)
	{
		if (_isBlank)
		{
			free(_pixel);
		}
		else
		{
			stbi_image_free(_pixel);
		}
	}
	_pixel = nullptr;
}

const math::Vector2i& Image::getSize() const
{
        return _size;
}

void Image::getPart(std::vector<unsigned char>& imagePart, Recti rect) const
{
	imagePart.resize((rect.w) * (rect.h) * _canalNumber);
	const unsigned int sizeByte = (rect.w) * _canalNumber;

	unsigned char* ptr = imagePart.data();
	for(int i = 0; i < rect.h; i++)
	{
		std::memcpy(ptr + i * sizeByte, _pixel + rect.x * _canalNumber + i * _size.x * _canalNumber, sizeByte);
	}
}

unsigned char* Image::getImagePtr()
{
	return _pixel;
}

size_t Image::GetDataSize() const
{
	return _size.x * _size.y * _canalNumber;
}

Image::Image(bool blank)
{
	_isBlank = blank;
}

bool Image::create(const math::Vector2i& inSize)
{
	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;
	_pixel = (unsigned char*)calloc(inSize.x * inSize.x * _canalNumber, sizeof(unsigned char));
	_size = inSize;
	return _pixel != nullptr;
}


bool Image::LoadImage()
{
	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;
	if (_path.GetExtension() == ".jpg")
	{
		_canalNumber = IMAGE_CANAL_NUMBER::RGB;
	}
	int c;
	_pixel = stbi_load(_path.GetPathString().c_str(), &_size.x, &_size.y, &c, _canalNumber);
	_canalNumber = (IMAGE_CANAL_NUMBER)c;
	if(_pixel == nullptr) return false;
	return true;
}


