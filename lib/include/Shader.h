#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

namespace fm {
	class Shader
	{
	public:
		GLuint Program;
		Shader();
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		Shader(const std::string &vertexCode, const std::string &fragmentCode);
		void Use();

		~Shader();
	};
}

