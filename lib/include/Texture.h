#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"
namespace fm {
	class Texture
	{
	public:
		Texture(const std::string &path, bool alpha = true);
		Texture();
		~Texture();
		void bind();
		void clear();
	private:
		int width;
		int height;
		int format;
		std::string path;

		Image image;

		GLuint id;
	};
}

//TODO soil free image