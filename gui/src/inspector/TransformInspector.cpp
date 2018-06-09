#include "inspector/TransformInspector.h"
#include "Components/CTransform.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Transform, fmc::CTransform)

void TransformInspector::draw() {
    if(ImGui::CollapsingHeader("Transform")) {
        ImGui::PushItemWidth(120);
        ImGui::DragFloat3("Position",
                          &target->position.x,
                          0.02f,
                          -FLT_MAX,
                          FLT_MAX,
                          NULL,
                          2.0f);
        ImGui::DragFloat3("Size", &target->scale.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);
        ImGui::DragFloat3("Rotation", &target->rotation.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);

        ImGui::DragInt("Layer", &target->layer, 1, 0, 99);
        ImGui::PopItemWidth();
    }
}


