#pragma once
#include <Core/Math/Vector2.h>
#include <Rendering/Mesh.hpp>
namespace fm {

    class StandardShapes {
    public:
        static rendering::MeshContainer* CreateQuad();
        static rendering::MeshContainer* CreateQuadFullScreen();

        static rendering::MeshContainer* CreateCircle();
        static rendering::MeshContainer* CreateCube();

    private:
        static void AddVertex(rendering::MeshContainer* mesh, const math::Vector3f &position, const fm::math::Vector2f& uv);
    };

}
