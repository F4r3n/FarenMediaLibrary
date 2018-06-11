#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
#include <Rendering/Shader.h>
#include <Resource/ResourcesManager.h>
using namespace fmc;
using namespace fm;
const std::string CMaterial::name = "Material";

CMaterial::CMaterial()
{
    color = fm::Color(1, 1, 1, 1);
}

CMaterial::CMaterial(const fm::Color &color, bool bloom)
{
    this->color = color;
    this->bloom = bloom;
}

CMaterial::~CMaterial()
{
}

void CMaterial::Reload()
{
    if(hasChanged)
    {
        if(shader == nullptr && shaderName != ""
          || shader && shaderName != shader->GetName())
        {
            shader =fm::ResourcesManager::get().getResource<fm::Shader>(shaderName);
        }

    }
    hasChanged = false;
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
    return name;
}

void CMaterial::setTexture(const fm::Texture& texture) {
    shaderName = "sprite";
    this->texture = texture;
    textureReady = true;
}
const fm::Texture& CMaterial::getTexture() {
    return texture;
}

void CMaterial::setValue(const std::string& name, int value) {
    fm::MaterialValue materialValue;
    materialValue = value;
    values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
}

const std::map<std::string, fm::MaterialValue>& CMaterial::getValues() const {
    return values;
}
