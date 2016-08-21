
#include "Texture.h"
#include "ResourcesManager.h"
#include <SOIL.h>
using namespace fm;
Texture::Texture(const std::string &path, bool alpha)
	:  path(path)
{
	if(alpha) {
		format = GL_RGBA;
	} else format = GL_RGB;


	image = SOIL_load_image(path.c_str(), &width, &height, 0, format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	//std::cout << "Texture " << texture.path << " Loaded " << texture.width << " " << texture.height << std::endl;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// Load image, create texture and generate mipmaps
	//unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

Texture::Texture() {

}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}


Texture::~Texture()
{
}
