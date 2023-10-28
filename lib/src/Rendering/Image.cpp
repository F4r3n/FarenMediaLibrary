#include "Rendering/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>
#include <nlohmann/json.hpp>
using namespace fm;
Image::Image(const fm::FilePath& inPath) : Resource(inPath)
{
	_path = inPath;
}

Image::~Image()
{
}

void Image::create(float width, float height)
{
}

void Image::clear()
{
	_size.reset();
	if (_pixel)
	{
		stbi_image_free(_pixel);
	}
	_pixel = nullptr;
}

const math::Vector2i& Image::getSize() const
{
        return _size;
}

void Image::getPart(std::vector<unsigned char>& imagePart, Recti rect) const
{
	imagePart.resize((rect.w) * (rect.h) * _canalNumber, 255);
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


bool Image::loadImage(const std::string& path)
{
	_pixel = stbi_load(path.c_str(), &_size.x, &_size.y, 0, STBI_rgb_alpha);
	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;
	if(_pixel == nullptr) return false;
	return true;
}


void Image::Load(const nlohmann::json& inJSON)
{
	Resource::Load(inJSON);
	_mipmapLevel = inJSON["params"]["mipmapLevel"];
	_canalNumber = inJSON["params"]["canalNumber"];
}

void Image::Save(nlohmann::json& outJSON) const
{
	Resource::Save(outJSON);

	outJSON["params"]["mipmapLevel"] = _mipmapLevel;
	outJSON["params"]["canalNumber"] = _canalNumber;
}
