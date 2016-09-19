#pragma once
#include "Vector2.h"
#include <vector>
#include <string>
#include "Rect.h"
namespace fm {
class Image {
public:
    Image();
    ~Image();
    void create(float width, float height);
    unsigned char* getImagePtr();
    void loadImage(const std::string& pathImage, const Vector2i& offset = { 0, 0 });
    const Vector2<int>& getSize() const;
    void clear();
    void getPart(std::vector<unsigned char>& imagePart, Recti rect) const;

private:
    unsigned char* _pixel = nullptr;
    Vector2<int> _size;
    Vector2<int> _offset;
    Vector2<int> _realSize;
    std::vector<unsigned char> pixels;
};
}