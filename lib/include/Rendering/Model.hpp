#pragma once
#include <Resource/Resource.h>
namespace fm {

namespace rendering {
    class MeshContainer;
    class VertexBuffer;
}

class Model : public Resource{
public:
    Model();
    ~Model();
    void generate();
    static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MESH;}

    rendering::MeshContainer* meshContainer;
    rendering::VertexBuffer* vertexBuffer;
    bool generated = false;
    std::string name = "";
};
}