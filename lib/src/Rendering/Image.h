#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include "Core/Rect.h"
namespace fm {
class Image {
public:

	enum IMAGE_CANAL_NUMBER
	{
		RGB = 3,
		RGBA = 4
	};

    Image();
    ~Image();
    void create(float width, float height);
    unsigned char* getImagePtr();
    bool loadImage(const std::string& pathImage);
    const math::Vector2i& getSize() const;
    void clear();
    void getPart(std::vector<unsigned char>& imagePart, Recti rect) const;
	size_t GetDataSize() const;
private:
    unsigned char* _pixel = nullptr;
    math::Vector2i _size;
    math::Vector2i _realSize;
	IMAGE_CANAL_NUMBER	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;
	
};
}