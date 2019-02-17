#include <Rendering/VertexBuffer.hpp>
#include <Core/Config.h>
using namespace fm;
using namespace rendering;
VertexBuffer::VertexBuffer() 
{
	_numberVertices = 0;
	_vaoIsSet = false;
}

VertexBuffer::~VertexBuffer() 
{
}


void VertexBuffer::destroy() 
{
    if(_indexVAO != 0)
    {
        glBindVertexArray(_indexVAO);
        if(_indexVBO != 0)
        {
            glDeleteBuffers(1, &_indexVBO);
        }
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &_indexVAO);
    }
}


void VertexBuffer::prepareData()
{
    glBindVertexArray(_indexVAO);
    if(_vaoIsSet) return;


    glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    _vaoIsSet = true;
}


void VertexBuffer::GenerateEmpty(size_t maxVertices)
{
    if(_indexVAO != 0)
    {
       destroy();
    }

    glGenVertexArrays(1, &_indexVAO);
    glBindVertexArray(_indexVAO);

    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
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
    if(_indexVAO == 0) return false;

    glBindVertexArray(_indexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(Vertex), number*sizeof(Vertex), inVertices);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return true;
}


void VertexBuffer::generate(const std::vector<Vertex>& vertices)
{
    glGenVertexArrays(1, &_indexVAO);
    glBindVertexArray(_indexVAO);

    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _numberVertices = vertices.size();
}


void VertexBuffer::generate() 
{

    glGenVertexArrays(1, &_indexVAO);
	glBindVertexArray(_indexVAO);

    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setBufferData(void* data, unsigned int size, unsigned int dataSize, bool staticData) 
{
    glBindVertexArray(_indexVAO);

    if(_numberVertices != size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _indexVBO);
        glBufferData(GL_ARRAY_BUFFER,
                 size*dataSize,
                 data,
                 staticData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _numberVertices = size;

     }
}

bool VertexBuffer::isGenerated()
{
    return _indexVBO != 0;
}
