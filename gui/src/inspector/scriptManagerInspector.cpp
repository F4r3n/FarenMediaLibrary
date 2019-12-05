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
		std::vector<std::string> scriptsToDelete;


		fmc::Scripts &&scripts = target->GetScripts();
		for (auto &&script : scripts)
		{
			std::string scriptName = script->GetScriptName() + "##" + std::to_string(target->GetIDEntity());
			bool toKeep = true;
			if (ImGui::CollapsingHeader(scriptName.c_str(), &toKeep))
			{

			}
			if (ImGui::Button("Reload"))
			{
				script->Reload();
			}
			if (!toKeep)
			{
				scriptsToDelete.emplace_back(script->GetScriptName());
			}
		}

		static char nameScript[128] = "ClassName";

		ImGui::InputText("Class Name", nameScript, IM_ARRAYSIZE(nameScript));
        if(ImGui::Button("Add Script"))
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

        if(!scriptsToDelete.empty())
        {
            for(auto &s : scriptsToDelete)
            {
                target->RemoveScript(s);
            }
        }
    }
}
