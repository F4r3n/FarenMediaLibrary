#pragma once
#include "Shape.h"
#include "Color.h"
#include "Texture.h"
namespace fm {
	class Sprite : public Shape
	{
	public:
		Sprite(Texture &texture, const Color &color = { 1, 1, 1});
		~Sprite();
		void draw(std::shared_ptr<Shader> shader);
	private:
		Texture texture;

		
		void initVertices(float r, float g, float b);
		void initBuffer();
	};
}

