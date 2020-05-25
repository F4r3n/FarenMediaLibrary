#include "Components/CMesh.h"
#include "Resource/ResourcesManager.h"
#include <memory>
#include "Core/Math/Functions.h"
#include <EntityManager.h>
#include "Rendering/Model.hpp"
#include <nlohmann/json.hpp>
using namespace fmc;

CMesh::~CMesh()
{
}

CMesh::CMesh() 
{
	_name = "Mesh";
	_type = "Quad";
	model = fm::ResourcesManager::get().getResource<fm::Model>(_type);
}

bool CMesh::Serialize(nlohmann::json &ioJson) const
{
    ioJson["type"] = _type;
    return true;
}
bool CMesh::Read(const nlohmann::json &inJSON)
{
    _type = inJSON["type"];
	model = fm::ResourcesManager::get().getResource<fm::Model>(_type);

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
    _type = type;
}

void CMesh::Destroy()
{
    EntityManager::get().removeComponent<CMesh>(BaseComponent::_IDEntity);
}

CMesh::CMesh(std::string type) {
    _type = type;
	_name = "Mesh";

}
