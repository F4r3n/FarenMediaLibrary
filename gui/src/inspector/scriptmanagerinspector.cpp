#include "inspector/scriptmanagerinspector.hpp"
#include <Components/CScriptManager.h>
#include <Script/LuaScript.h>
#include <Script/cppscript.hpp>
#include <imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(ScriptManager, fmc::CScriptManager)

void ScriptManagerInspector::draw()
{
    bool value = true;
    if(ImGui::CollapsingHeader("ScriptManagerInspector", &value))
    {
        //ImGui::ColorEdit3("Color", &target->color.r);
        for(auto &script : target->scripts)
        {
            if(script->GetType() == fm::Script::SCRIPT_TYPE::CPP)
            {
                std::shared_ptr<fm::CppScript> cppScript = std::static_pointer_cast<fm::CppScript>(script);
                //Use serialization to draw
               ImGui::Text(cppScript->GetNameClass().c_str());

            }
            else if(script->GetType() == fm::Script::SCRIPT_TYPE::LUA)
            {
                std::shared_ptr<fm::LuaScript> luaScript = std::static_pointer_cast<fm::LuaScript>(script);

            }

        }

        static int current = 0;
        static const char *shapeNames[] = {"LUA", "CPP"};


        ImGui::PushItemWidth(120);
        ImGui::Combo("##Type", &current, shapeNames, 2);
        ImGui::PopItemWidth();
        static char nameScript[128] = "ClassName";
        if(current == fm::Script::SCRIPT_TYPE::CPP)
        {
            ImGui::InputText("input text", nameScript, IM_ARRAYSIZE(nameScript));

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
    }
}
