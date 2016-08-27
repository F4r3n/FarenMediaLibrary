#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace fm;
Image::Image() {}

Image::~Image() {
	if(_pixel) stbi_image_free(_pixel);

}

void Image::create(float width, float height) {

}

Vector2<int> Image::getSize() {
	return _size;
}

unsigned char* Image::getImagePtr() { return _pixel;}

void Image::loadImage(const std::string &path) {
	_pixel = stbi_load(path.c_str(), &_size.x, &_size.y, 0, STBI_rgb_alpha);
	pixels.clear();
	
}



