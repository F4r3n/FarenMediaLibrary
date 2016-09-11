#pragma once
#include "Shader.h"
#include "Effect.h"
#include <memory>
namespace fm {
	class Drawable : public Effect
	{
	public:
		Drawable();
		~Drawable();
		virtual void draw(std::shared_ptr<Shader> shader) = 0;
		virtual const std::string getNameShader() const = 0;
	};
}

