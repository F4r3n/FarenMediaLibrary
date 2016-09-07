#pragma once
#include "Shape.h"
#include "Shader.h"
#include "Color.h"
namespace fm {
	class Rectangle :public Shape
	{
	public:
		Rectangle();
		Rectangle(Color color);
		Rectangle(float r, float g, float b);
		virtual ~Rectangle();
		void draw(Shader &shader);

	private:
		
		void initBuffer();
		void initVertices(float r, float g, float b);

		GLfloat vertices[20];
		
	};
}

