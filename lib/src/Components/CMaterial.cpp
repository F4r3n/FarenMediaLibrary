#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
#include <Rendering/Shader.h>
#include <Resource/ResourcesManager.h>
#include <EntityManager.h>
#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
using namespace fmc;
using namespace fm;


CMaterial::CMaterial()
{
	_name = "Material";
   _materials.push_back(fm::ResourcesManager::get().getResource<fm::Material>("default_material"));
}


CMaterial::~CMaterial()
{
}

void CMaterial::Destroy()
{
    EntityManager::get().removeComponent<CMaterial>(BaseComponent::_IDEntity);
}



bool CMaterial::Serialize(nlohmann::json &ioJson) const
{
    nlohmann::json j;
    for(fm::Material *s : _materials)
    {
        nlohmann::json m = s->GetName();
       j.push_back(m);
    }
    ioJson["Materials"] = j;
    return true;
}

bool CMaterial::Read(const nlohmann::json &inJSON)
{
	for (const auto& m : inJSON["Materials"])
	{
		//Material material
	}
    return true;
}


const std::string& CMaterial::GetName() const
{
    return _name;
}


