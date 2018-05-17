#include "MeshInspector.h"
#include <imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Mesh, fmc::CMesh)

void MeshInspector::draw()
{
    bool value = true;
    static int current = 0;
    static const char *shapeNames[] = {"Quad", "Circle"};

    if(ImGui::CollapsingHeader("Mesh", value))
    {
        ImGui::PushItemWidth(120);
        ImGui::Combo("##Shape", &current, shapeNames, 2);
        ImGui::PopItemWidth();
        target->SetModelType(shapeNames[current]);
        // t->setShape(current);
    }
}
