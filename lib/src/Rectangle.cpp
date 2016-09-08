
#include "Rectangle.h"

using namespace fm;

Rectangle::Rectangle()
{
	nameShader = "default";

	initVertices(1, 1, 1);

	initBuffer();
	
}

void Rectangle::initBuffer() {
	unsigned int indices[] = { 0,1,2,0,3,1 };
	indicesSize = 6;

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
}

Rectangle::Rectangle(Color color) {
	nameShader = "default";

	initVertices(color.r, color.g, color.b);
	setColor(color);
	initBuffer();
}
void Rectangle::initVertices(float r, float g, float b) {
	vertices[0] = 0.0f; vertices[1] = 1.0f;
	vertices[2] = r; vertices[3] = g; vertices[4] = b;

	vertices[5] = 1.0f; vertices[6] = 0.0f;
	vertices[7] = r; vertices[8] = g; vertices[9] = b;

	vertices[10] = 0.0f; vertices[11] = 0.0f;
	vertices[12] = r; vertices[13] = g; vertices[14] =b;


	vertices[15] = 1.0f; vertices[16] = 1.0f;
	vertices[17] = r; vertices[18] = g; vertices[19] = b;
}


Rectangle::Rectangle(float r, float g, float b) {
	nameShader = "default";

	initVertices(r, g, b);
	setColor({r, g, b});
	initBuffer();
	
}

void Rectangle::draw(Shader &shader) {
	
	Shape::draw(shader);
}


Rectangle::~Rectangle()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

}
