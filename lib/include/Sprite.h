#pragma once
#include "Shape.h"
namespace fm {
	class Sprite : public Shape
	{
	public:
		Sprite(const std::string &textureName);
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

