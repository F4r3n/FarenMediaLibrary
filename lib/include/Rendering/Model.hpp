#pragma once

namespace fm {

namespace rendering {
    class MeshContainer;
    class IndexBuffer;
}

class Model {
public:
    Model();
    ~Model();
    void generate();

    rendering::MeshContainer* meshContainer;
    rendering::IndexBuffer* indexBuffer;
    bool generated = false;
};
}