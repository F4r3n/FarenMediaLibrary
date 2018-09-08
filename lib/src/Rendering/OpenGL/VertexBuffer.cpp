#include <Rendering/VertexBuffer.hpp>
#include <Core/Config.h>
using namespace fm;
using namespace rendering;
VertexBuffer::VertexBuffer() {
}

VertexBuffer::~VertexBuffer() {
}

void VertexBuffer::destroy() {

    if(indexVAO != 0)
    {
        glBindVertexArray(indexVAO);
        if(index != 0)
        {
            glDeleteBuffers(1, &index);
        }
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &indexVAO);
    }


}

void VertexBuffer::prepareData()
{
#if !OPENGL_ES || OPENGL_ES_VERSION > 2//TODO why binding each time, if we use VAO ???
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

void VertexBuffer::GenerateEmpty(size_t maxVertices)
{
    if(indexVAO != 0)
    {
       destroy();
    }

    glGenVertexArrays(1, &indexVAO);
    glBindVertexArray(indexVAO);

    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER, maxVertices*sizeof(Vertex), nullptr, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

bool VertexBuffer::AddVertices(Vertex *inVertices, size_t number, size_t offset)
{
    if(indexVAO == 0) return false;

    glBindVertexArray(indexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(Vertex), number*sizeof(Vertex), inVertices);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return true;
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
    if(numberVertices != size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, index);
        glBufferData(GL_ARRAY_BUFFER,
                 size*dataSize,
                 data,
                 staticData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        numberVertices = size;

     }
}

bool VertexBuffer::isGenerated() {
    return index != 0;
    
}
