#include <Rendering/StandardShapes.h>
#include <Core/Math/Functions.h>
namespace fm {
using namespace rendering;
void StandardShapes::AddVertex(MeshContainer* mesh,
                               const fm::math::Vector2f& position,
                               const fm::math::Vector2f& uv) {
    mesh->vertices.push_back({position, uv});
}

MeshContainer* StandardShapes::CreateQuad() {
    MeshContainer* meshContainer = new MeshContainer();
    AddVertex(meshContainer, {0, 1}, {0.0, 1.0});
    AddVertex(meshContainer, {0, 0}, {0.0, 0.0});
    AddVertex(meshContainer, {1, 0}, {1.0, 0.0});
    AddVertex(meshContainer, {1, 1}, {1.0, 1.0});
    meshContainer->listIndices = {0, 1, 2, 0, 2, 3};

    return meshContainer;
}

MeshContainer* StandardShapes::CreateQuadFullScreen() {
    MeshContainer* meshContainer = new MeshContainer();
    AddVertex(meshContainer, {-1, 1}, {0.0, 0.0});
    AddVertex(meshContainer, {-1, -1}, {0.0, 1.0});
    AddVertex(meshContainer, {1, -1}, {1.0, 1.0});
    AddVertex(meshContainer, {1, 1}, {1.0, 0.0});
    meshContainer->listIndices = {0, 1, 2, 0, 2, 3};

    return meshContainer;
}

MeshContainer* StandardShapes::CreateCircle() {
    MeshContainer* meshContainer = new MeshContainer();

    unsigned int numberVertices = 100;
    float intervall = 2 * fm::math::pi() / numberVertices;

    AddVertex(meshContainer, {0.5, 0.5}, {0.5, 0.5});

    for(float teta = 0; teta < 2 * fm::math::pi(); teta += intervall) {
        fm::math::Vector2f uv((float)(0.5 + cos(teta) / 2),
                              (float)(0.5 + sin(teta) / 2.0f));
        AddVertex(meshContainer, uv, uv);
    }

    int j = 0;
    for(unsigned int i = 1; i < numberVertices; ++i) {
        j = i;
        meshContainer->listIndices.push_back(j++);
        meshContainer->listIndices.push_back(j);
        meshContainer->listIndices.push_back(0);
    }
    meshContainer->listIndices.push_back(j);
    meshContainer->listIndices.push_back(1);
    meshContainer->listIndices.push_back(0);

    return meshContainer;
}
}
