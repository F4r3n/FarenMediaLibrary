#include "Components/CMaterial.h"
using namespace fmc;
const std::string CMaterial::name = "Material";

CMaterial::CMaterial() {
    color = fm::Color(1, 1, 1, 1);
    _name = "Material";
}

CMaterial::CMaterial(const fm::Color &color, bool bloom) {
    this->color = color;
    this->bloom = bloom;
    _name = "Material";
}

CMaterial::~CMaterial() {
}

void CMaterial::setTexture(const fm::Texture& texture) {
    shaderName = "sprite";
    this->texture = texture;
    textureReady = true;
}
const fm::Texture& CMaterial::getTexture() {
    return texture;
}