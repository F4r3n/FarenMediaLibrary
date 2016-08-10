#include "stdafx.h"
#include "Sprite.h"
#include "ResourcesManager.h"
using namespace fm;

Sprite::Sprite(const std::string &textureName)
{
	this->textureName = textureName;
	ResourcesManager::loadTexture(textureName, textureName);
	initVertices(1, 1, 1);
	initBuffer();
}

void Sprite::initVertices(float r, float g, float b) {
	vertices[0] = 0.0f; vertices[1] = 1.0f;
	//vertices[0] = 0.0f; vertices[1] = 1.0f;
	vertices[2] = r; vertices[3] = g; vertices[4] = b;

	vertices[5] = 1.0f; vertices[6] = 0.0f;
	vertices[7] = r; vertices[8] = g; vertices[9] = b;

	vertices[10] = 0.0f; vertices[11] = 0.0f;
	vertices[12] = r; vertices[13] = g; vertices[14] = b;


	vertices[15] = 1.0f; vertices[16] = 1.0f;
	vertices[17] = r; vertices[18] = g; vertices[19] = b;
}

void Sprite::initBuffer() {
	unsigned int indices[] = { 0,1,2,0,3,1 };
	indicesSize = 6;
	nameShader = "sprite";


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	verticesSize = 6;
	glBindVertexArray(0); // Unbind VAO


						  // Load and create a texture 
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	//unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	Texture2D texture2d = ResourcesManager::getTexture(textureName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2d.width, texture2d.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2d.image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(texture2dimage);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

void Sprite::draw(Shader &shader) {
	glBindTexture(GL_TEXTURE_2D, texture);
	Shape::draw(shader);
}


Sprite::~Sprite()
{
}
