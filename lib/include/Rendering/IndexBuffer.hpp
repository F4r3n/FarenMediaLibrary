#pragma once
#include <Core/Math/Vector2.h>
#include <vector>
namespace fm {
namespace rendering { 
    struct Vertex {
        fm::math::Vector2f position;
        fm::math::Vector2f uv;
    };
    
    class IndexBuffer {
    public :
        IndexBuffer();
        ~IndexBuffer();
        void generate(const std::vector<Vertex> &vertices);
        
                //Used by opengl
        unsigned int index;
    private:

    };

}
    
}