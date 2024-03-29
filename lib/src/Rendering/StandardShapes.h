#pragma once
#include <Core/Math/Vector2.h>
#include <Core/Math/Vector3.h>
#include "GraphicsAPI.h"
namespace fm
{
	namespace rendering
	{
		struct MeshContainer;
	}
}

namespace fm {

    class StandardShapes {
    public:
        static rendering::MeshContainer* CreateQuad();
        static rendering::MeshContainer* CreateQuadFullScreen();

        static rendering::MeshContainer* CreateCircle();
        static rendering::MeshContainer* CreateCube();

		static rendering::MeshContainer* CreateTriangle(GRAPHIC_API inAPI);

    private:
        static void AddVertex(rendering::MeshContainer* mesh, const math::Vector3f &position, const fm::math::Vector2f& uv, const math::Vector3f &normals);
    };

}
