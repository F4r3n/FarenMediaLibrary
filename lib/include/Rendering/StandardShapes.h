#pragma once
#include <Core/Math/Vector2.h>
#include <Rendering/Mesh.hpp>
namespace fm {

    class StandardShapes {
    public:
        static rendering::MeshContainer* CreateQuad();
        static rendering::MeshContainer* CreateQuadFullScreen();

        static rendering::MeshContainer* CreateCircle();

    private:
        static void AddVertex(rendering::MeshContainer* mesh, const fm::math::Vector2f& position, const fm::math::Vector2f& uv);
    };

}