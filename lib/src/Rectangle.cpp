
#include "Rectangle.h"

using namespace fm;

Rectangle::Rectangle()
{

	initVertices(1, 1, 1);

	initBuffer();
	
}

void Rectangle::initBuffer() {
	mesh.create();

}

Rectangle::Rectangle(Color color) {
	initVertices(color.r, color.g, color.b);
	setColor(color);
	initBuffer();
}
void Rectangle::initVertices(float r, float g, float b) {
	mesh.setShaderName("default");

	mesh.addVertex({0.0,1.0}, {0.0,1.0});
	mesh.addVertex({0.0,0.0}, {1.0,0.0});
	mesh.addVertex({1.0,0.0}, {0.0,0.0});
	mesh.addVertex({1.0,1.0}, {1.0,1.0});
	std::vector<unsigned int> v = { 0,1,2,0,2,3 };
	mesh.setIndices(v);
}


Rectangle::Rectangle(float r, float g, float b) {

	initVertices(r, g, b);
	setColor({r, g, b});
	initBuffer();
	
}

void Rectangle::draw(std::shared_ptr<Shader> shader) {
	
	Shape::draw(shader);
}


Rectangle::~Rectangle()
{}
