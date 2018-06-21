#include "Rendering/batch.hpp"

using namespace fm;

Batch::Batch() {}

Batch::~Batch() {}


void Batch::Init()
{
    _vertexBuffer.GenerateEmpty(_MAX_VERTICES);
}

void Batch::Clear()
{
    _vertexBuffer.destroy();
}

bool Batch::Add(rendering::Vertex *vertices, size_t number)
{
    if(_currentNumberVertices + number > _MAX_VERTICES) return false;
    if(_vertexBuffer.AddVertices(vertices, number, _currentNumberVertices))
    {
        _currentNumberVertices += number;
        return true;
    }

    return false;
}
