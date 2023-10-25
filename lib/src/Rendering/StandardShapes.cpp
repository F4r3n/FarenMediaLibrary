#include <Rendering/StandardShapes.h>
#include <Core/Math/Functions.h>
#include <Rendering/Mesh.hpp>

namespace fm {
using namespace rendering;
void StandardShapes::AddVertex(MeshContainer* mesh,
                               const fm::math::Vector3f& position,
                               const fm::math::Vector2f& uv,
                               const fm::math::Vector3f& normals
                               ) {
	mesh->vertices.push_back({ position, {1.0f, 1.0f, 1.0f}, uv, normals });
}

MeshContainer* StandardShapes::CreateQuad()
{
    MeshContainer* meshContainer = new MeshContainer();

    AddVertex(meshContainer, {-0.5, 0.5, 0}, {0.0, 1.0}, {0.0,0.0,-1.0});
    AddVertex(meshContainer, {-0.5f, -0.5f, 0}, {0.0, 0.0},{0.0,0.0,-1.0});
    AddVertex(meshContainer, {0.5, -0.5, 0}, {1.0, 0.0},{0.0,0.0,-1.0});
    AddVertex(meshContainer, {0.5, 0.5, 0}, {1.0, 1.0},{0.0,0.0,-1.0});
    meshContainer->listIndices = {0, 1, 2, 0, 2, 3};

    return meshContainer;
}

MeshContainer* StandardShapes::CreateTriangle(GRAPHIC_API inAPI)
{
	MeshContainer* meshContainer = new MeshContainer();
	if (inAPI == GRAPHIC_API::VULKAN)
	{
		AddVertex(meshContainer, { 1.0, 1.0, 0 }, { 0.0, 1.0 }, { 0.0,0.0,-1.0 });
		AddVertex(meshContainer, { -1.0, 1.0, 0 }, { 0.0, 0.0 }, { 0.0,0.0,-1.0 });
		AddVertex(meshContainer, { 0.0f, -1.0f, 0 }, { 1.0, 0.0 }, { 0.0,0.0,-1.0 });
	}
	else if (inAPI == GRAPHIC_API::OPENGL)
	{

	}
	meshContainer->listIndices = { 0, 1, 2 };


	return meshContainer;
}


MeshContainer* StandardShapes::CreateCube()
{
    MeshContainer* meshContainer = new MeshContainer();

    //Back
    AddVertex(meshContainer, {-0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});
    AddVertex(meshContainer, {-0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f,  0.0f,  1.0f});

	//Front
	AddVertex(meshContainer, { -0.5f, -0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });
	AddVertex(meshContainer, { 0.5f, -0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });
	AddVertex(meshContainer, { 0.5f,  0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });
	AddVertex(meshContainer, { 0.5f,  0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });
	AddVertex(meshContainer, { -0.5f,  0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });
	AddVertex(meshContainer, { -0.5f, -0.5f, -0.5f }, { 0.0,0.0 }, { 0.0f,  0.0f, -1.0f });

    //Left
    AddVertex(meshContainer, {-0.5f,  0.5f,  0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f,  0.5f, -0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f, -0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f, -0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f,  0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f,  0.5f,  0.5f},{0.0,0.0},{-1.0f,  0.0f,  0.0f});

    //Right
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f, -0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f, -0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f, -0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f,  0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 1.0f,  0.0f,  0.0f});

    //Bottom
    AddVertex(meshContainer, {-0.5f, -0.5f, -0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f, -0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f,  0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f, -0.5f, -0.5f},{0.0,0.0},{ 0.0f, -1.0f,  0.0f});

    //Top
    AddVertex(meshContainer, {-0.5f,  0.5f, -0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f, -0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});
    AddVertex(meshContainer, { 0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f,  0.5f,  0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});
    AddVertex(meshContainer, {-0.5f,  0.5f, -0.5f},{0.0,0.0},{ 0.0f,  1.0f,  0.0f});


    meshContainer->listIndices = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};

    return meshContainer;
}


MeshContainer* StandardShapes::CreateQuadFullScreen()
{
    MeshContainer* meshContainer = new MeshContainer();
    AddVertex(meshContainer, {-1, 1, 0}, {0.0, 0.0}, {0.0, 0.0, 0.0});
    AddVertex(meshContainer, {-1, -1, 0}, {0.0, 1.0}, {0.0, 0.0, 0.0});
    AddVertex(meshContainer, {1, -1, 0}, {1.0, 1.0}, {0.0, 0.0, 0.0});
    AddVertex(meshContainer, {1, 1, 0}, {1.0, 0.0}, {0.0, 0.0, 0.0});
    meshContainer->listIndices = {0, 1, 2, 0, 2, 3};

    return meshContainer;
}

MeshContainer* StandardShapes::CreateCircle()
{
    MeshContainer* meshContainer = new MeshContainer();

    unsigned int numberVertices = 100;
    float intervall = float(2 * fm::math::pi() / numberVertices);

    AddVertex(meshContainer, {0.5, 0.5, 0}, {0.5, 0.5}, {0.0,0.0,0.0});

    for(float teta = 0; teta < 2 * fm::math::pi(); teta += intervall) {
        fm::math::Vector2f uv((float)(0.5 + cos(teta) / 2),
                              (float)(0.5 + sin(teta) / 2.0f));
        AddVertex(meshContainer, uv, uv, {0.0, 0.0,-1.0});
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
