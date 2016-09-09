#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include <GL/glew.h>
#include <iostream>
namespace fm {

	class Mesh {

	struct Vertex {
		Vector2f position;
		Vector2f uv;
	};


	public:
		Mesh();
		~Mesh();
		void addVertex(const Vector2f &position, const Vector2f &uv = {0,0});
		void create();
		void setShaderName(const std::string &name);
		void setIndices(const std::vector<unsigned int> &listIndices);
		void draw();
		const std::string getShaderName() const;
	private:
		std::vector<Vertex> vertices;
		std::string shaderName = "default";
		std::vector<unsigned int> listIndices;

		GLuint VAO, VBO, EBO;
	};

}