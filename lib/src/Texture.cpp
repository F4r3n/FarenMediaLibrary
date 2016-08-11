#include "stdafx.h"
#include "Texture.h"
#include "ResourcesManager.h"
#include <SOIL.h>
using namespace fm;
Texture::Texture(const std::string &name, const std::string &path)
	:  name(name), path(path)
{
	image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	//std::cout << "Texture " << texture.path << " Loaded " << texture.width << " " << texture.height << std::endl;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
											  // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	//unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(texture2dimage);
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
