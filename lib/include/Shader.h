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
		void setMatrix(const std::string &name, glm::mat4 matrix);
		void setVector2f(const std::string &name, glm::vec2 vector);
		void setVector3f(const std::string &name, glm::vec3 vector);
		void setVector4f(const std::string &name, glm::vec4 vector);
		void setFloat(const std::string &name, float val);
		void Use();

		~Shader();
	private:
		void init(const std::string &vertexCode, const std::string &fragmentCode);
	};
}

