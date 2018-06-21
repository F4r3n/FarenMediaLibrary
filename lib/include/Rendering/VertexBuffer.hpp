#pragma once
#include <Core/Math/Vector2.h>
#include <vector>
#include "Rendering/OpenGL/OGLGraphicsDef.hpp"
#include <Rendering/Mesh.hpp>
namespace fm {
namespace rendering {

    
    class VertexBuffer {
    public :
        VertexBuffer();
        ~VertexBuffer();
        void generate(const std::vector<Vertex> &vertices);
        void generate();
        void setBufferData(void* data, unsigned int size, unsigned int dataSize, bool staticData = true);
        void prepareData();
        void destroy();
        bool isGenerated();
        void GenerateEmpty(size_t maxVertices);
        bool AddVertices(Vertex *inVertices, size_t number, size_t offset);
                //Used by opengl
        unsigned int index;
        unsigned int indexVAO;
        unsigned int numberVertices = 0;
    private:
        //const unsigned int MAX_VERTICES = 1000;
        //unsigned int _currentNumberVertices = 0;

    };

}
    
}
