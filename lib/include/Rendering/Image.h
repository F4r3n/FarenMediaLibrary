#pragma once
#include "Core/Math/Vector2.h"
#include <vector>
#include <string>
#include "Core/Rect.h"
namespace fm {
class Image {
public:
    Image();
    ~Image();
    void create(float width, float height);
    unsigned char* getImagePtr();
    bool loadImage(const std::string& pathImage, const math::Vector2i& offset = { 0, 0 });
    const math::Vector2i& getSize() const;
    void clear();
    void getPart(std::vector<unsigned char>& imagePart, Recti rect) const;

private:
    unsigned char* _pixel = nullptr;
    math::Vector2i _size;
    math::Vector2i _offset;
    math::Vector2i _realSize;
    std::vector<unsigned char> pixels;
};
}