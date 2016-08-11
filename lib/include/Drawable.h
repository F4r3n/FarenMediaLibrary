#pragma once
#include "Shader.h"
namespace fm {
	class Drawable
	{
	public:
		Drawable();
		~Drawable();
		virtual void draw(Shader &shader) = 0;
		virtual const std::string getNameShader() const = 0;
	};
}

