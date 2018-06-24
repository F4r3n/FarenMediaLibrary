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
    color = fm::Color(1, 1, 1, 1);
}

CMaterial::CMaterial(const fm::Color &color, bool bloom)
{
    this->color = color;
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
        if((shader == nullptr && shaderName != "")
          || (shader && shaderName != shader->GetName()))
        {
            shader = fm::ResourcesManager::get().getResource<fm::Shader>(shaderName);
            _hasChanged = false;
            return true;
        }

    }

    _hasChanged = false;
    return false;

}

bool CMaterial::Serialize(json &ioJson) const
{
    ioJson["shaderName"] = shaderName;
    ioJson["color"] = color;
    return true;
}

bool CMaterial::Read(const json &inJSON)
{
    shaderName = inJSON["shaderName"];
    color = inJSON["color"];
    return true;
}


const std::string& CMaterial::GetName() const
{
    return kName;
}


void CMaterial::setValue(const std::string& name, int value) {
    fm::MaterialValue materialValue;
    materialValue = value;
    values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
}

const std::map<std::string, fm::MaterialValue>& CMaterial::getValues() const {
    return values;
}
