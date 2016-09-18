#pragma once
#include "Component.h"
#include "Color.h"
#include "Texture.h"
namespace fmc {
class CMaterial : public Component<CMaterial> {
public:
    CMaterial();
    ~CMaterial();
    fm::Color color;
    fm::Texture texture;
    std::string shaderName = "default";
};
}