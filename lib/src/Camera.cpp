
#include "Camera.h"
#include "ResourcesManager.h"
#include "Tag.h"
using namespace fm;
Camera::Camera(int width, int height):width(width), height(height) {
	projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

	viewPort.width = width;
	viewPort.height = height;
	viewPort.x = 0;
	viewPort.y = 0;
}

Camera::Camera(int width, int height, const ViewPort &view, float zoom) : width(width), height(height) {
	projection = glm::ortho(0.0f, (float)width*zoom, (float)height*zoom, 0.0f);

	viewPort.width = view.width;
	viewPort.height = view.height;
	viewPort.x = view.x;
	viewPort.y = view.y;
}

Camera::Camera() {}

void Camera::setTag(int tag) {
	this->tag = tag;
}

void Camera::view() {
	glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
}

void Camera::draw(Drawable &shape)
{
	//if (!Tag::checkTag(shape.tag, tag)) return;
	Shader s = ResourcesManager::getShader(shape.getNameShader());
	s.Use();
	s.setMatrix("projection", projection);
	s.setMatrix("view", viewMatrix);
	shape.draw(s);
}



glm::mat4 Camera::getProjection() const{
	return projection;
}

void Camera::moveTo(int x, int y) {
	this->x = -x;
	this->y = -y;
	viewPort.x = this->x;
	viewPort.y = this->y;
}

void Camera::rotate(float angle) {
	this->angle = angle;
}

void Camera::apply() {
	viewMatrix = glm::mat4();
	viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, 0));

	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, angle, glm::vec3(0, 0, 1));
	viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));
}

Camera::~Camera()
{
}
