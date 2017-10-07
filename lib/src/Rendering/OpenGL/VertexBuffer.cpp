#include <Rendering/VertexBuffer.hpp>
#include <Core/Config.h>
using namespace fm;
using namespace rendering;
VertexBuffer::VertexBuffer() {
    
}

VertexBuffer::~VertexBuffer() {
    
}

void VertexBuffer::destroy() {
    glDeleteBuffers(1, &index);
}

void VertexBuffer::generate(const std::vector<Vertex> &vertices) {
    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
