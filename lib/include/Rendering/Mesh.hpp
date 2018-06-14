#pragma once
#include <vector>
#include <Core/Math/Vector2.h>
#include <Core/Math/Vector3.h>

namespace fm {

namespace rendering {
    struct Vertex {
        fm::math::Vector3f position;
        fm::math::Vector2f uv;
        fm::math::Vector3f normals;
    };

    struct MeshContainer {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> listIndices;
    };
}
}
