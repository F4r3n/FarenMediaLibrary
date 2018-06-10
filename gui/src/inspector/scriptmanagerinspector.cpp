#include "inspector/scriptmanagerinspector.hpp"
#include <Components/CScriptManager.h>
#include <Script/LuaScript.h>
#include <Script/cppscript.hpp>
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(ScriptManager, fmc::CScriptManager)

void ScriptManagerInspector::draw(bool *value)
{
    if(ImGui::CollapsingHeader("ScriptManagerInspector", value))
    {


        static int current = 0;
        static const char *shapeNames[] = {"LUA", "CPP"};


        ImGui::PushItemWidth(120);
        ImGui::Combo("##Type", &current, shapeNames, 2);
        ImGui::PopItemWidth();
        static char nameScript[128] = "ClassName";
        if(current == fm::Script::SCRIPT_TYPE::CPP)
        {
            ImGui::InputText("Class Name", nameScript, IM_ARRAYSIZE(nameScript));

        }else if(current == fm::Script::SCRIPT_TYPE::LUA)
        {

        }

        if(ImGui::Button("Add Script"))
        {
            if(current == fm::Script::SCRIPT_TYPE::CPP)
            {
                target->addScript(std::make_shared<fm::CppScript>(nameScript));
            }

        }
        std::vector<std::string> scriptsToDelete;
        //ImGui::ColorEdit3("Color", &target->color.r);
        for(auto &script : target->scripts)
        {
            const char* t = script->GetScriptName().c_str();
           ImGui::BulletText("%s",t);
           ImGui::SameLine();
           if(ImGui::SmallButton("x"))
           {
              scriptsToDelete.emplace_back(std::string(t));
           }

        }
        if(!scriptsToDelete.empty())
        {
            std::cout << scriptsToDelete.size() << std::endl;
            for(auto &s : scriptsToDelete)
            {
                target->RemoveScript(s);
            }
        }
    }
}
