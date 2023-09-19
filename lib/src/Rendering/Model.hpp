#pragma once
#include <Resource/Resource.h>
#include <vector>
#include <memory>
#include "Core/Bounds.h"
#include "GraphicsAPI.h"

namespace fm {

namespace rendering
{
    struct MeshContainer;
    class VertexBuffer;
}

struct Mesh
{
    rendering::MeshContainer* meshContainer;
    std::unique_ptr<rendering::VertexBuffer> vertexBuffer;
};

class Model : public Resource
{
public:

    Model(const fm::FilePath &inFilePath);
	Model() = delete;
    ~Model();
    static const fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MESH;}
	virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

    void AddMesh(rendering::MeshContainer* inMeshContainer);

    void PrepareBuffer();
    const std::string& GetName() const {return _name;}
    size_t GetNumberMeshes() {return _meshes.size();}
    rendering::MeshContainer* GetMeshContainer(size_t index) {return _meshes[index].meshContainer;}
	void BindIndex(size_t index) const;
	void Reload(bool force = false) {}

	uint32_t GetID() const { return _currentID; }
	void Destroy();
public:
	std::function<void()> _destroyCallback = {};
    std::vector<Mesh> _meshes;//One model may have more than one mesh
    std::string _name = "";
	fm::Bounds _bounds;
	inline static uint32_t _ID = 0;

	uint32_t	_currentID = 0;

};
}
