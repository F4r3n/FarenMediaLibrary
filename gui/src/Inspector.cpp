#include "Inspector.hpp"
#include <imgui.h>
#include <Components/CCamera.h>
#include <Components/CMaterial.h>
#include <Components/Body2D.h>
#include <Components/CDirectionalLight.h>
#include <Components/CIdentity.h>
#include <Components/CTransform.h>
#include <Components/CMaterial.h>
#include <Components/CPointLight.h>
#include <Components/CMesh.h>
#include <Components/CText.h>
Inspector::Inspector()
{
}

Inspector::~Inspector()
{
}

void Inspector::OnDraw(fmc::CMesh *mesh, bool *value)
{
    static int current = 0;
    static const char *shapeNames[] = {"Quad", "Circle"};

    if(ImGui::CollapsingHeader("Mesh", value))
    {
        ImGui::PushItemWidth(120);
        ImGui::Combo("##Shape", &current, shapeNames, 2);
        ImGui::PopItemWidth();
        mesh->SetModelType(shapeNames[current]);
        // t->setShape(current);
    }
}
void Inspector::OnDraw(fmc::CMaterial *material, bool *value)
{
    if(ImGui::CollapsingHeader("Material", value))
    {
        ImGui::ColorEdit3("Color", &material->color.r);
    }
}
void Inspector::OnDraw(fmc::Body2D *t, bool *value)
{
    static bool isDynamic = false;
    if(ImGui::CollapsingHeader("Body2D", value))
    {
        ImGui::DragFloat2(
            "Size##Body", &t->size.x, 0.02f, 0, FLT_MAX, NULL, 2.0f);
        if(ImGui::Checkbox("Is dynamic", &isDynamic))
        {
            t->isDynamic = isDynamic;
            if(isDynamic)
            {
                t->bodyDef.type = b2_dynamicBody;
            } else
            {
                t->bodyDef.type = b2_staticBody;
            }
        }
    }
}
void Inspector::OnDraw(fmc::CDirectionalLight *t, bool *value)
{
    if(ImGui::CollapsingHeader("Directional Light"))
    {
        ImGui::ColorEdit3("Color", &t->color.r);
    }
}
void Inspector::OnDraw(fmc::CTransform *t, bool *value)
{
    if(ImGui::CollapsingHeader("Transform"))
    {
        ImGui::PushItemWidth(100);
        ImGui::DragFloat2(
            "Position", &t->position.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);
        ImGui::DragFloat2(
            "Size", &t->scale.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);
        ImGui::DragInt("Layer", &t->layer, 1, 0, 99);
        ImGui::PopItemWidth();
    }
}

void Inspector::OnDraw(fmc::CText *ctext, bool *value)
{
    if(ImGui::CollapsingHeader("Text Renderer", value))
    {
        static char textToRender[256];
        ImGui::InputText("Text", textToRender, 256);
        ImGui::DragFloat("Size Text", &ctext->scale, 0.1, 0, 10);
        ctext->text = std::string(textToRender);
    }
}
