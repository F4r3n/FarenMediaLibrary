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

void VertexBuffer::prepareData()
{
#if !OPENGL_ES || OPENGL_ES_VERSION > 2
    glBindVertexArray(indexVAO);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

}


void VertexBuffer::generate(const std::vector<Vertex>& vertices) {
#if !OPENGL_ES || OPENGL_ES_VERSION > 2

    glGenVertexArrays(1, &indexVAO);
    glBindVertexArray(indexVAO);
#endif
    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    numberVertices = vertices.size();
}

void VertexBuffer::generate() {
#if !OPENGL_ES || OPENGL_ES_VERSION > 2

    glGenVertexArrays(1, &indexVAO);
    glBindVertexArray(indexVAO);
#endif

    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    // glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setBufferData(void* data,
                                 unsigned int size,
                                 unsigned int dataSize,
                                 bool staticData) {
#if !OPENGL_ES || OPENGL_ES_VERSION > 2
    glBindVertexArray(indexVAO);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER,
                 size*dataSize,
                 data,
                 staticData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    numberVertices = size;
}

bool VertexBuffer::isGenerated() {
    return index != 0;
    
}
