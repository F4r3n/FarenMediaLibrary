#include <Rendering/VertexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>
#include <Core/Debug.h>


using namespace fm;
using namespace rendering;

Model::Model(const fm::FilePath& inFilePath): Resource(inFilePath)
{
    _name = inFilePath.GetName(true);
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

void Model::BindIndex(size_t index) const
{
	_meshes[index].vertexBuffer->Bind();
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
