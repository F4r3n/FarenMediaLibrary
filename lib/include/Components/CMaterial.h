#pragma once
#include "Component.h"
#include "Color.h"
#include "Texture.h"
namespace fmc {
class CMaterial : public Component<CMaterial> {
public:
    CMaterial();
    ~CMaterial();
    CMaterial(const fm::Color &color, bool bloom);
    void setTexture(const fm::Texture& texture);
    const fm::Texture& getTexture();
    bool textureReady = false;
    std::string shaderName = "default";
    fm::Color color;
    bool bloom = false;
    
    #ifdef GUI
    void display(bool *value) {
        if(ImGui::CollapsingHeader("Material", value)) {
            ImGui::ColorEdit3("Color", &color.r);
        }
    }
    #endif
    static const std::string name;
private:
    fm::Texture texture;
};
}