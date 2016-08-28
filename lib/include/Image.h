#pragma once
#include "Vector2.h"
#include <vector>
#include <string>

namespace fm {
	class Image {
		public:
			Image();
			~Image();
			void create(float width, float height);
			unsigned char* getImagePtr();
			void loadImage(const std::string &pathImage, const Vector2i &offset = {0,0});
			Vector2<int> getSize();
			void clear();
		private:
			unsigned char* _pixel = nullptr;
			Vector2<int> _size;
			Vector2<int> _offset;
			Vector2<int> _realSize;
			std::vector<unsigned char> pixels;

	};
}