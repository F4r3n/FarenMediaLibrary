#include <Rendering/IndexBuffer.hpp>
#include <Rendering/StandardShapes.h>
#include <Rendering/Model.hpp>

using namespace fm;
using namespace rendering;

Model::Model(): Resource() {
    indexBuffer = new IndexBuffer();
}

Model::~Model() {
    if(indexBuffer != nullptr) {
        delete indexBuffer;
        indexBuffer = nullptr;
    }
}

void Model::generate() {
    if(indexBuffer != nullptr) {
        indexBuffer->generate(meshContainer->vertices);
        generated = true;
    }
}