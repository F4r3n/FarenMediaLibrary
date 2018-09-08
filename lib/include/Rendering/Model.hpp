#pragma once
#include <Resource/Resource.h>
#include <vector>
#include <memory>
namespace fm {

namespace rendering {
    class MeshContainer;
    class VertexBuffer;
}

struct Mesh
{
    rendering::MeshContainer* meshContainer;
    std::unique_ptr<rendering::VertexBuffer> vertexBuffer;
};

class Model : public Resource{
public:
    Model(const std::string &inName);
    ~Model();
    void generate();
    static const fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MESH;}

    void AddMesh(rendering::MeshContainer* inMeshContainer);

    void PrepareBuffer();
    //const std::vector<Mesh>& GetMeshes() const {return _meshes;}
    const std::string& GetName() const {return _name;}
    size_t GetNumberMeshes() {return _meshes.size();}
    rendering::MeshContainer* GetMeshContainer(size_t index) {return _meshes[index].meshContainer;}
private:
    std::vector<Mesh> _meshes;//One model may have more than one mesh

    bool _generated = false;
    std::string _name = "";
};
}
