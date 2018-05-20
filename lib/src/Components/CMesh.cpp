#include "Components/CMesh.h"
#include <iostream>
#include "Resource/ResourcesManager.h"
#include <memory>
#include "Core/Math/Functions.h"
using namespace fmc;

CMesh::~CMesh()
{
}

CMesh::CMesh() {
    _name = "Mesh";
}

bool CMesh::Serialize(json &ioJson) const
{
    return false;
}
bool CMesh::Read(const json &inJSON)
{
    return false;
}

const std::string& CMesh::GetName() const
{
    return _name;
}

bool CMesh::IsmodelReady() {
    return model != nullptr;
}

void CMesh::SetType(const std::string &type) {
    this->type = type;
}

CMesh::CMesh(std::string type) {
    this->type = type;
}
