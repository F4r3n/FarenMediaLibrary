#include <Rendering/VertexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>

using namespace fm;
using namespace rendering;

Model::Model(): Resource() {
    vertexBuffer = new VertexBuffer();
}

Model::~Model() {
    if(vertexBuffer != nullptr) {
        delete vertexBuffer;
        vertexBuffer = nullptr;
    }
}

void Model::generate() {
    if(vertexBuffer != nullptr) {
        vertexBuffer->generate(meshContainer->vertices);
        generated = true;
    }
}