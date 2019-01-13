#include "inspector/scriptManagerInspector.hpp"
#include <Components/CScriptManager.h>
#include <Script/LuaScript.h>
#include <Script/cppscript.hpp>
#include <imgui/imgui.h>
#include "Core/FilePath.h"
#include "Resource/ResourcesManager.h"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(ScriptManager, fmc::CScriptManager)

void ScriptManagerInspector::init()
{

}


void ScriptManagerInspector::draw(bool *value)
{
    if(ImGui::CollapsingHeader("ScriptManagerInspector", value))
    {


        static int current = 0;
        static const char *shapeNames[] = {"LUA", "CPP"};


        ImGui::PushItemWidth(120);
        ImGui::Combo("##Type", &current, shapeNames, 2);
        ImGui::PopItemWidth();
        if(current == fm::Script::SCRIPT_TYPE::CPP)
        {
            ImGui::InputText("Class Name", nameScript, IM_ARRAYSIZE(nameScript));

        }else if(current == fm::Script::SCRIPT_TYPE::LUA)
        {
			ImGui::InputText("Class Name", nameScript, IM_ARRAYSIZE(nameScript));
        }

        if(ImGui::Button("Add Script"))
        {
            if(current == fm::Script::SCRIPT_TYPE::CPP)
            {
             //   target->addScript(std::make_shared<fm::CppScript>(nameScript));
            }
			else if (current == fm::Script::SCRIPT_TYPE::LUA)
			{
				fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::USER_LUA_LOCATION);
				if (p.GetPath().empty())
				{
					p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::INTERNAL_RESOURCES_LOCATION);
				}
				std::string fileName = std::string(nameScript) + ".lua";
				p.Append(fileName);
				
				target->addScriptLua(p);
			}


        }
        std::vector<std::string> scriptsToDelete;
        //ImGui::ColorEdit3("Color", &target->color.r);
        for(auto &script : target->scripts)
        {
            const char* t = script->GetScriptName().c_str();
           ImGui::BulletText("%s",t);
           ImGui::SameLine();

		   if (ImGui::Button("Reload"))
		   {
			   script->Reload();
		   }
		   ImGui::SameLine();

           if(ImGui::SmallButton("X"))
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
