#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
using namespace fmc;
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

bool CMaterial::Serialize(json &ioJson) const
{
    ioJson["shaderName"] = shaderName;
    json o;
    fm::Serialize(&color, o, (size_t)3);
    ioJson["color"] = o;
    return true;
}
bool CMaterial::Read(const json &inJSON)
{
    return false;
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
