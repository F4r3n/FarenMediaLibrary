#include "Rendering/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>
#define IMAGE_RGBA 4
#define IMAGE_RGB 3
#include <iostream>
using namespace fm;
Image::Image() {
}

Image::~Image() {
}

void Image::create(float width, float height) {
    pixels.resize(width * height * IMAGE_RGBA, 255);
}

void Image::clear() {
    _size.reset();
    if(_pixel)
        stbi_image_free(_pixel);
}

const math::Vector2i& Image::getSize() const {
    return _size;
}

void Image::getPart(std::vector<unsigned char>& imagePart, Recti rect) const{
    imagePart.resize((rect.w) * (rect.h) * IMAGE_RGBA, 255);
    unsigned int sizeByte = (rect.w) * IMAGE_RGBA;

    unsigned char* ptr = imagePart.data();
    for(int i = 0; i < rect.h; i++) {

        std::memcpy(ptr + i * sizeByte, _pixel + rect.x * IMAGE_RGBA + i * _size.x * IMAGE_RGBA, sizeByte);
    }
}

unsigned char* Image::getImagePtr() {
    return _pixel;
}

bool Image::loadImage(const std::string& path, const math::Vector2i& offset) {
    pixels.clear();
    _offset = offset;
   
    _pixel = stbi_load(path.c_str(), &_size.x, &_size.y, 0, STBI_rgb_alpha);
    if(_pixel == nullptr) return false;
    return true;
}
