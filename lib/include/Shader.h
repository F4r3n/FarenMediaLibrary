#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace fm {
	class Shader
	{
	public:
		GLuint Program;
		Shader();
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		Shader(const std::string &vertexCode, const std::string &fragmentCode);
		Shader* setMatrix(const std::string &name, glm::mat4 matrix);
		Shader* setVector2f(const std::string &name, glm::vec2 vector);
		Shader* setVector3f(const std::string &name, glm::vec3 vector);
		Shader* setVector4f(const std::string &name, glm::vec4 vector);
		Shader* setFloat(const std::string &name, float val);
		Shader* setInt(const std::string &name, int val);
		Shader* Use();
		bool compile();
		~Shader();
	private:
		
		std::string vertex, fragment;
	};
}

