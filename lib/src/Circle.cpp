
#include "Circle.h"

using namespace fm;
Circle::Circle()
{
}

void Circle::initBuffer(std::vector<unsigned int> &indices) {
	
	nameShader = "default";
	
	indicesSize = indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	verticesSize = vertices.size();
	glBindVertexArray(0); // Unbind VAO
}

Circle::Circle(Color color, int radius, int number) {
	this->numberVertices = number;
	this->radius = radius;
	scaleX = radius;
	scaleY = radius;

	initVertices(color.r / 255, color.g / 255, color.b / 255);
	
	
}
void Circle::initVertices(float r, float g, float b) {

	float intervall = 2*glm::pi<float>()/numberVertices;
	std::vector<unsigned int> indices;
	
	//Center
	vertices.push_back(0); vertices.push_back(0);
	vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);


	for (float teta = 0; teta < 2 * glm::pi<float>(); teta += intervall) {
		vertices.push_back(glm::cos(teta)); vertices.push_back(glm::sin(teta));
		vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
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
