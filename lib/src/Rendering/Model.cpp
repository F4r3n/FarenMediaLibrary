#include <Rendering/VertexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>
#include <Core/Debug.h>

using namespace fm;
using namespace rendering;

Model::Model(const std::string &inName): Resource()
{
    _name = inName;
}


void Model::AddMesh(MeshContainer *inMeshContainer)
{
    _meshes.push_back({inMeshContainer, nullptr});
}



Model::~Model()
{
    for(auto &mesh : _meshes)
    {
       delete mesh.meshContainer;
    }
}

void Model::PrepareBuffer()
{
    for(auto &mesh : _meshes)
    {
        if(mesh.vertexBuffer == nullptr)
        {
            fm::Debug::get().LogError("Vertex buffer is empty and should be prepared ???");
        }

        mesh.vertexBuffer->prepareData();
    }
}

void Model::generate()
{

    for(auto &mesh : _meshes)
    {
        if(mesh.vertexBuffer == nullptr)
        {
            mesh.vertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer());
        }

        mesh.vertexBuffer->generate(mesh.meshContainer->vertices);
    }
}
