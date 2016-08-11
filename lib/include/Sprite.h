#pragma once
#include "Shape.h"
#include "Color.h"
#include "Texture.h"
namespace fm {
	class Sprite : public Shape
	{
	public:
		Sprite(Texture &texture, const Color &color = { 255, 255, 255});
		~Sprite();
		void draw(Shader &shader);
	private:
		Texture texture;

		GLfloat vertices[20];
		void initVertices(float r, float g, float b);
		void initBuffer();
	};
}

