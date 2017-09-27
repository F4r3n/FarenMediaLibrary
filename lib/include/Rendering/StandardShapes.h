#pragma once
#include <Core/Math/Vector2.h>
#include <Rendering/IndexBuffer.hpp>
namespace fm {

namespace rendering {


    struct MeshContainer {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> listIndices;
    };

    class StandardShapes {
    public:
        static MeshContainer* CreateQuad();
        static MeshContainer* CreateCircle();

    private:
        static void AddVertex(MeshContainer* mesh, const fm::math::Vector2f& position, const fm::math::Vector2f& uv);
    };
}
}