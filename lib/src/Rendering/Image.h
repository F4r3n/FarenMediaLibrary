#pragma once
#include "Core/Math/Vector2.h"
#include <string>
#include "Core/Rect.h"
#include "Resource/Resource.h"
#include "nlohmann/json_fwd.hpp"
#include "Core/FilePath.h"
namespace fm
{
	class Image
	{
	public:

		enum IMAGE_CANAL_NUMBER : int
		{
			R = 1,
			RG = 2,
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

		unsigned char* GetPtr() const { return _pixel; }
		IMAGE_CANAL_NUMBER GetCanalNumber() const { return _canalNumber; }
	private:
		size_t				_mipmapLevel = 0;
		IMAGE_CANAL_NUMBER	_canalNumber = IMAGE_CANAL_NUMBER::RGBA;

		unsigned char* _pixel = nullptr;
		math::Vector2i _size;
		bool		   _isBlank = false;
		fm::FilePath	_path;
	};
}