#ifndef BATCH_HPP
#define BATCH_HPP
#include <Rendering/VertexBuffer.hpp>
namespace fm
{
class Batch
{
 public:
        Batch();
        ~Batch();
        void Init();
        void Clear();
        bool Add(rendering::Vertex *vertices, size_t number);
  private:
        rendering::VertexBuffer _vertexBuffer;
        const unsigned int _MAX_VERTICES = 1000;
        unsigned int _currentNumberVertices = 0;
};
}

#endif // BATCH_HPP

