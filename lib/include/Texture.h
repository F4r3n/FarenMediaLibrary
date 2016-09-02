#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"
#include "Rect.h"
namespace fm {
	class Texture
	{
	public:
		Texture(const std::string &path, Recti rect = {0,0,-1,-1}, bool alpha = true);
		Texture();
		~Texture();
		void bind();
		void clear();
	private:

		int format;
		std::string path;
		std::vector<unsigned char> content;

		GLuint id;
	};
}

//TODO soil free image