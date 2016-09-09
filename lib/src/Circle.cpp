
#include "Circle.h"

using namespace fm;
Circle::Circle()
{
}

void Circle::initBuffer(std::vector<unsigned int> &indices) {
	mesh.setIndices(indices);
	mesh.create();
}

Circle::Circle(const Color &color, int radius, int number) {
	this->numberVertices = number;
	this->radius = radius;
	scaleX = radius;
	scaleY = radius;
	setColor(color);
	initVertices(color.r, color.g, color.b);
	
	
}
void Circle::initVertices(float r, float g, float b) {

	float intervall = 2*glm::pi<float>()/numberVertices;
	std::vector<unsigned int> indices;
	
	//Center
	mesh.addVertex({0,0});
	//vertices.push_back(0); vertices.push_back(0);
	//vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);


	for (float teta = 0; teta < 2 * glm::pi<float>(); teta += intervall) {
		mesh.addVertex({glm::cos(teta), glm::sin(teta)});
		//vertices.push_back(glm::cos(teta)); vertices.push_back(glm::sin(teta));
		//vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
	}

	int j = 0;
	for (int i = 1; i < numberVertices; ++i) {
		j = i;
		indices.push_back(j++);
		indices.push_back(j);
		indices.push_back(0);
		
	}
	indices.push_back(j); indices.push_back(1); indices.push_back(0);

	initBuffer(indices);
}

Circle::~Circle()
{
}
