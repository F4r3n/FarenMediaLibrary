#include "CMaterial.h"
using namespace fmc;
CMaterial::CMaterial() {
    color = fm::Color(1, 1, 1, 1);
}

CMaterial::CMaterial(const fm::Color &color, bool bloom) {
    this->color = color;
    this->bloom = bloom;
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