#include "inspector/MeshInspector.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Mesh, fmc::CMesh)

void MeshInspector::draw(bool *value)
{
    static int current = 0;
    static const char *shapeNames[] = {"Quad", "Circle", "Cube"};

    if(ImGui::CollapsingHeader("Mesh", value))
    {
        ImGui::PushItemWidth(120);
        ImGui::Combo("##Shape", &current, shapeNames, 3);
        ImGui::PopItemWidth();
        target->SetModelType(shapeNames[current]);
        // t->setShape(current);
    }
}
