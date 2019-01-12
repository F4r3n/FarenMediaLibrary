#include "inspector/Inspector.hpp"
#include <imgui/imgui.h>

#include <Components/CDirectionalLight.h>

Inspector::Inspector()
{
}

Inspector::~Inspector()
{
}

void Inspector::OnDraw(fmc::CDirectionalLight *t, bool *)
{
    if(ImGui::CollapsingHeader("Directional Light"))
    {
        ImGui::ColorEdit3("Color", &t->color.r);
    }
}

/*

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
*/
