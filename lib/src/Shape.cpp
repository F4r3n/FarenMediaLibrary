
#include "Shape.h"
using namespace fm;
Shape::Shape()
{
	model = glm::mat4();
	posX = 0;
	posY = 0;
	scaleX = 100;
	scaleY = 100;
}

void Shape::draw(Shader &shader) {
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(posX, posY,  0.0f));


	model = glm::translate(model, glm::vec3(0.5f * scaleX, 0.5f * scaleY, 0.0f));
	model = glm::rotate(model, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * scaleX, -0.5f * scaleY, 0.0f));

	model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
	shader.setMatrix("model", model)->setVector4f("mainColor", glm::vec4(color.r, color.g, color.b, color.a));
	mesh.draw();

}

void Shape::setColor(const Color &color) {
	this->color = color;
}

Vector2i Shape::getPosition() const {
	return {posX, posY};
}


void Shape::rotate(float angle) {
	if(angle != angle) return;
	rotateAngle = angle;
}

void Shape::scale(int x, int y) {
	scaleX = x; scaleY = y;
}

void Shape::move(int x, int y) {
	posX = x; posY = y;
}

void Shape::setTag(int tag) {
	this->tag = tag;
}

const std::string Shape::getNameShader() const {
	return mesh.getShaderName();
}

Shape::~Shape()
{
}
