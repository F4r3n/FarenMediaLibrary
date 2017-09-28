#pragma once
#include <Core/Math/Vector2.h>
#include <vector>
#include <Rendering/Mesh.hpp>
namespace fm {
namespace rendering { 

    
    class IndexBuffer {
    public :
        IndexBuffer();
        ~IndexBuffer();
        void generate(const std::vector<Vertex> &vertices);
        void destroy();
                //Used by opengl
        unsigned int index;
    private:

    };

}
    
}