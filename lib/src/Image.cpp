#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>
#define IMAGE_RGBA 4
#define IMAGE_RGB 3
#include <iostream>
using namespace fm;
Image::Image() {}

Image::~Image() {

}

void Image::create(float width, float height) {
	pixels.resize(width*height*IMAGE_RGBA, 255);
}

void Image::clear() {
	_size.reset();
	if(_pixel) stbi_image_free(_pixel);
}

Vector2<int> Image::getSize() {
	return _realSize;
}

unsigned char* Image::getImagePtr() { return pixels.data();}

void Image::loadImage(const std::string &path, const Vector2i &offset) {
	pixels.clear();
	_offset = offset;

	_pixel = stbi_load(path.c_str(), &_size.x, &_size.y, 0, STBI_rgb_alpha);
	pixels.resize((_size.x - offset.x)*(_size.y - offset.y)*IMAGE_RGBA, 200);
	int sizeByte = (_size.x - offset.x)*4;

	_realSize.x = _size.x - _offset.x;
	_realSize.y = _size.y - _offset.y;


	unsigned char* ptr = pixels.data();
	for(int i = 0; i < _size.y - offset.y; i++){
		std::memcpy(ptr + i*sizeByte, 
			_pixel + offset.x*4 + i*_size.x*4 , 
				sizeByte);

	}
	
}



