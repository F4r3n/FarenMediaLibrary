
#include "Sprite.h"
using namespace fm;

Sprite::Sprite(Texture &texture, const Color &color)
{
	this->texture = texture;
	setColor(color);
	initVertices(color.r/255, color.g/255, color.b/255);
	initBuffer();
}

void Sprite::initVertices(float r, float g, float b) {
	mesh.setShaderName("sprite");

	mesh.addVertex({0.0,1.0}, {0.0,1.0});
	mesh.addVertex({0.0,0.0}, {0.0,0.0});
	mesh.addVertex({1.0,0.0}, {1.0,0.0});
	mesh.addVertex({1.0,1.0}, {1.0,1.0});
	std::vector<unsigned int> v = { 0,1,2,0,2,3 };
	mesh.setIndices(v);
}

void Sprite::initBuffer() {
	mesh.create();
}

void Sprite::draw(Shader &shader) {
	glActiveTexture(GL_TEXTURE0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	texture.bind();


	Shape::draw(shader);
}


Sprite::~Sprite()
{

}
