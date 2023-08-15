#include <Rendering/VertexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>
#include "Rendering/OpenGL/OGLVertexBuffer.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include <Core/Debug.h>


using namespace fm;
using namespace rendering;

Model::Model(const fm::FilePath& inFilePath): Resource(inFilePath)
{
    _name = inFilePath.GetName(true);
	_ID++;
	_currentID = _ID;
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
	if (auto v = dynamic_cast<fm::rendering::OGLVertextBuffer*>(_meshes[index].vertexBuffer.get()))
		v->Bind();
}

void Model::PrepareBuffer()
{
    for(auto &mesh : _meshes)
    {
        if(mesh.vertexBuffer == nullptr)
        {
            fm::Debug::get().LogError("Vertex buffer is empty and should be prepared ???");
        }
		if (auto v = dynamic_cast<fm::rendering::OGLVertextBuffer*>(mesh.vertexBuffer.get()))
		{
			v->prepareData();
		}
    }
}

void Model::generate(GRAPHIC_API inAPI)
{
    for(auto &mesh : _meshes)
    {
        if(mesh.vertexBuffer == nullptr)
        {
			if (inAPI == GRAPHIC_API::OPENGL)
			{
				mesh.vertexBuffer = std::unique_ptr<VertexBuffer>(new OGLVertextBuffer());
			}
			else if (inAPI == GRAPHIC_API::VULKAN)
			{
				mesh.vertexBuffer = std::unique_ptr<VertexBuffer>(new fm::VkVertexBuffer());
			}
        }

		mesh.vertexBuffer->UploadData(*mesh.meshContainer);
    }
}
