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
        void setBufferData(void* data, unsigned int size, bool staticData = true);
        void destroy();
                //Used by opengl
        unsigned int index;
        
    private:

    };

}
    
}