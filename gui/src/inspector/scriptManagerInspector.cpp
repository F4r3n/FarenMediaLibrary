#include "inspector/scriptManagerInspector.hpp"
#include <Components/CScriptManager.h>
#include <Script/LuaScript.h>
#include <Script/cppscript.hpp>
#include <imgui/imgui.h>
#include "Core/FilePath.h"
#include "Resource/ResourcesManager.h"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(ScriptManager, fmc::CScriptManager)

void ScriptManagerInspector::_Init()
{
}

void ScriptManagerInspector::_DeInit()
{
}

void ScriptManagerInspector::Draw(bool *value)
{
    if(ImGui::CollapsingHeader("ScriptManagerInspector", value))
    {
		std::vector<std::string> scriptsToDelete;


		fmc::LuaScripts &&scripts = _target->GetLuaScripts();
		for (auto &&script : scripts)
		{
			std::string scriptName = script->GetScriptName() + "##" + std::to_string(_target->GetIDEntity());
			bool toKeep = true;
			if (ImGui::CollapsingHeader(scriptName.c_str(), &toKeep))
			{

			}
			if (ImGui::Button("Reload"))
			{
				_target->ReloadScript(script->GetScriptName());
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
			
			_target->addScriptLua(p);
			
        }

        if(!scriptsToDelete.empty())
        {
            for(auto &s : scriptsToDelete)
            {
                _target->RemoveScript(s);
            }
        }
    }
}
