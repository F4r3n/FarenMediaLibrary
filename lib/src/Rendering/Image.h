#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include "Core/Rect.h"
#include "Resource/Resource.h"
#include "nlohmann/json_fwd.hpp"
#include "Core/FilePath.h"
namespace fm {
class Image : public Resource {
public:

	enum IMAGE_CANAL_NUMBER
	{
		RGB = 3,
		RGBA = 4
	};

    Image(const fm::FilePath& inPath);
	Image(bool blank);
    ~Image();
    unsigned char* getImagePtr();
	bool create(const math::Vector2i& inSize);
    bool LoadImage();
    const math::Vector2i& getSize() const;
    void clear();
    void getPart(std::vector<unsigned char>& imagePart, Recti rect) const;
	size_t GetDataSize() const;
	static fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::IMAGE; }
	virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

	void Load(const nlohmann::json& inJSON) override;
	void Save(nlohmann::json& outJSON) const override;
	unsigned char* GetPtr() const { return _pixel; }
private:
	size_t				_mipmapLevel = 0;
	IMAGE_CANAL_NUMBER	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;

    unsigned char* _pixel = nullptr;
    math::Vector2i _size;
	bool		   _isBlank = false;
};
}