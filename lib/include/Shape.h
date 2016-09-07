#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Drawable.h"
#include "Tag.h"
#include "Vector2.h"
namespace fm {
	class Shape : public Drawable
	{
	public:
		Shape();
		~Shape();
		virtual void draw(Shader &shader);
		int tag = tg::TAG_DEFAULT;
		void setTag(int tag);


		void move(int x, int y);
		template <typename T>
		void move(Vector2<T> v) {
			posX = v.x;
			posY = v.y;
		}
		void scale(int width, int height);
		void rotate(float angle);
		const std::string getNameShader() const;
		Vector2i getPosition() const;
		
	protected:
		GLuint VBO, VAO, EBO;
		unsigned int verticesSize;
		unsigned int indicesSize;
		std::string nameShader;
		
		int scaleX = 1, scaleY = 1;
	private:
		glm::mat4 model;
		int posX = 0, posY = 0;
		
		float rotateAngle = 0;
	};
}

