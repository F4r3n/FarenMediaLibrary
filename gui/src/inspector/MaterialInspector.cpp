#include "inspector/MaterialInspector.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)

void MaterialInspector::draw() {
    bool value = true;
    if(ImGui::CollapsingHeader("Material", &value)) {
        ImGui::ColorEdit3("Color", &target->color.r);
    }
}
