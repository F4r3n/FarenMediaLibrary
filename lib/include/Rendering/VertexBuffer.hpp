#pragma once
#include <Core/Math/Vector2.h>
#include <vector>
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
                //Used by opengl
        unsigned int index;
        unsigned int indexVAO;
        unsigned int numberVertices = 0;
    private:

    };

}
    
}