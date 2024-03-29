#include <Rendering/VertexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>
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


void Model::Destroy()
{
	if (_destroyCallback != nullptr)
		_destroyCallback();
}


