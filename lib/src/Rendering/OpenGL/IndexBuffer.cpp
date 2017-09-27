#include <Rendering/IndexBuffer.hpp>
#include <Core/Config.h>
using namespace fm;
using namespace rendering;
IndexBuffer::IndexBuffer() {
    
}

IndexBuffer::~IndexBuffer() {
    
}

void IndexBuffer::generate(const std::vector<Vertex> &vertices) {
    glGenBuffers(GL_ARRAY_BUFFER, &index);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}
