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
			void loadImage(const std::string &pathImage);
			Vector2<int> getSize();
		private:
			unsigned char* _pixel = nullptr;
			Vector2<int> _size;
			std::vector<unsigned char> pixels;
	};
}