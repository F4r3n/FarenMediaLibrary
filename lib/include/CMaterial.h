#pragma once
#include "Component.h"
#include "Color.h"
#include "Texture.h"
namespace fmc {
class CMaterial : public Component<CMaterial> {
public:
    CMaterial();
    ~CMaterial();
    void setTexture(const fm::Texture& texture);
    const fm::Texture& getTexture();
    bool textureReady = false;
    std::string shaderName = "default";
    fm::Color color;

private:
    fm::Texture texture;
};
}