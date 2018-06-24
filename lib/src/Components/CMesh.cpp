#include "Components/CMesh.h"
#include <iostream>
#include "Resource/ResourcesManager.h"
#include <memory>
#include "Core/Math/Functions.h"
#include <EntityManager.h>
using namespace fmc;

CMesh::~CMesh()
{
}

CMesh::CMesh() {
    _name = "Mesh";
}

bool CMesh::Serialize(json &ioJson) const
{
    ioJson["type"] = type;
    return true;
}
bool CMesh::Read(const json &inJSON)
{
    type = inJSON["type"];

    return true;
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

void CMesh::Destroy()
{
    EntityManager::get().removeComponent<CMesh>(BaseComponent::_IDEntity);
}

CMesh::CMesh(std::string type) {
    this->type = type;
}
