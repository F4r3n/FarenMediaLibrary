#pragma once
#include <vector>
#include <Core/Math/Vector2.h>
#include <Core/Math/Vector3.h>

namespace fm {

namespace rendering
{
    struct Vertex
	{
        fm::math::Vector3f position;
		fm::math::Vector3f color;
        fm::math::Vector2f uv;
        fm::math::Vector3f normal;
    };

    struct MeshContainer
	{
        std::vector<Vertex> vertices;
        std::vector<uint32_t> listIndices; //Do not forget to change it: VK_INDEX_TYPE_UINT32
    };
}
}
