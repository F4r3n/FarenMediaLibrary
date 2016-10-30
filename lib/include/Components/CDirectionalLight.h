#pragma once
#include <ECS.h>
#include "Color.h"
#ifdef GUI
#include <imgui.h>
#endif
namespace fmc {
class CDirectionalLight : public Component<CDirectionalLight>{
public:
    CDirectionalLight();
    CDirectionalLight(const fm::Color &color);
    ~CDirectionalLight();
    fm::Color color;
    static const std::string name;
    
    #ifdef GUI
    void display(bool *value) {
         if(ImGui::CollapsingHeader(name.c_str())) {
              ImGui::ColorEdit3("Color", &color.r);
         }
    }
    #endif
};
}