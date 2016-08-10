#pragma once
#include "Shape.h"
#include "Color.h"
namespace fm {
	class Sprite : public Shape
	{
	public:
		Sprite(const std::string &textureName, const Color &color = { 255, 255, 255});
		~Sprite();
		void draw(Shader &shader);
	private:
		GLuint texture;
		std::string textureName = "";
		GLfloat vertices[20];
		void initVertices(float r, float g, float b);
		void initBuffer();
	};
}

