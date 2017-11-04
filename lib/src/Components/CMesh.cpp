#include "Components/CMesh.h"
#include <iostream>
#include "Resource/ResourcesManager.h"
#include <memory>
#include "Core/Math/Functions.h"
using namespace fmc;
const std::string CMesh::name = "Mesh";

CMesh::~CMesh() {
    // destroy();
}

CMesh::CMesh() {
    _name = "Mesh";
   // _name("Mesh");
}

bool CMesh::IsmodelReady() {
    return model != nullptr;
}

void CMesh::setType(const std::string &type) {
    this->type = type;
}

CMesh::CMesh(std::string type) {
    this->type = type;
}
