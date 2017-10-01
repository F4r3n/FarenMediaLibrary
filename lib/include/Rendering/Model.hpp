#pragma once
#include <Resource/Resource.h>
namespace fm {

namespace rendering {
    class MeshContainer;
    class IndexBuffer;
}

class Model : public Resource{
public:
    Model();
    ~Model();
    void generate();
    static fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MESH;}

    rendering::MeshContainer* meshContainer;
    rendering::IndexBuffer* indexBuffer;
    bool generated = false;
};
}