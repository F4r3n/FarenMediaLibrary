#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
#include <Rendering/Shader.h>
#include <Resource/ResourcesManager.h>
#include <EntityManager.h>

using namespace fmc;
using namespace fm;
static const std::string kName = "Material";


CMaterial::CMaterial()
{
   _materials.push_back(fm::Material());
}


CMaterial::~CMaterial()
{
}

void CMaterial::Destroy()
{
    EntityManager::get().removeComponent<CMaterial>(BaseComponent::_IDEntity);
}

bool CMaterial::Reload()
{
    if(_hasChanged)
    {
        for(auto &m : _materials)
        {
            m.Reload();
        }
        return true;
    }

    _hasChanged = false;
    return false;

}

bool CMaterial::Serialize(json &ioJson) const
{
    nlohmann::json j;
   //for(auto &s : _materials)
   //{
   //    nlohmann::json m = s;
   //   j.push_back(m);
   //}
   //ioJson["Materials"] = j;
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


