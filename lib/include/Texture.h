#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fm {
	class Texture
	{
	public:
		Texture(const std::string &name, const std::string &path);
		Texture();
		~Texture();
		void bind();
	private:
		int width;
		int height;
		std::string name;
		std::string path;

		unsigned char* image;

		GLuint id;
	};
}

//TODO soil free image