#pragma once

namespace fm {

	class Color
	{
	public:
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		~Color();
		 float r = 0, g = 0, b = 0;
		 float a = 255;
		 float mean();
		 
	};
}

