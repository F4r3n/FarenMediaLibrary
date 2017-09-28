#pragma once
#include <vector>
#include <Core/Math/Vector2.h>
namespace fm {

namespace rendering {
    struct Vertex {
        fm::math::Vector2f position;
        fm::math::Vector2f uv;
    };

    struct MeshContainer {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> listIndices;
    };
}
}