#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
#include <Rendering/Shader.h>
#include <Resource/ResourcesManager.h>
#include <EntityManager.h>
#include "Rendering/material.hpp"

using namespace fmc;
using namespace fm;
static const std::string kName = "Material";


CMaterial::CMaterial()
{
   _materials.push_back(fm::ResourcesManager::get().getResource<fm::Material>("default_material"));
}


CMaterial::~CMaterial()
{
}

void CMaterial::Destroy()
{
    EntityManager::get().removeComponent<CMaterial>(BaseComponent::_IDEntity);
}

void CMaterial::SetFlagHasChanged() {
    for(auto &s: _materials) s->SetFlagHasChanged();
}


bool CMaterial::Reload()
{
    if(_hasChanged)
    {
        for(auto &m : _materials)
        {
            m->Reload();
        }
        return true;
    }

    _hasChanged = false;
    return false;

}

bool CMaterial::Serialize(json &ioJson) const
{
    nlohmann::json j;
    for(fm::Material *s : _materials)
    {
        nlohmann::json m = s->GetID();
       j.push_back(m);
    }
    ioJson["Materials"] = j;
    return true;
}

bool CMaterial::Read(const json &inJSON)
{

    return true;
}


const std::string& CMaterial::GetName() const
{
    return kName;
}


