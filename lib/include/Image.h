#pragma once

namespace fm {
	class Image {
		public:
			Image();
			~Image();
			void create(float width, float height);
			unsigned char* getImagePtr();
		private:
			unsigned char* _pixel;
	};
}