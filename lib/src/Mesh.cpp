#include "Mesh.h"

using namespace fm;
Mesh::Mesh() {}

Mesh::~Mesh() {

	glDeleteBuffers(1, &VBO);
	if(!listIndices.empty())
    	glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::create() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	if(!listIndices.empty())
		glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	if(!listIndices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*listIndices.size(), listIndices.data(), GL_STATIC_DRAW);
	}

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Mesh::draw() {
	glBindVertexArray(VAO);
	if(!listIndices.empty())
		glDrawElements(GL_TRIANGLES, listIndices.size(), GL_UNSIGNED_INT, 0);
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	glBindVertexArray(0); // Unbind VAO
}

void Mesh::addVertex(const Vector2f &position, const Vector2f &uv) {
	Vertex v{{position.x, position.y}, {uv.x, uv.y}};
	vertices.push_back(v);
}

void Mesh::setShaderName(const std::string &name) {
	shaderName = name;
}

const std::string Mesh::getShaderName() const {
	return shaderName;
}

void Mesh::setIndices(const std::vector<unsigned int> &listIndices) {
	this->listIndices = listIndices;
}
