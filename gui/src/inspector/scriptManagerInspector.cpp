#include "inspector/scriptManagerInspector.hpp"
#include <FML/Components/CScriptManager.h>
#include <Script/LuaScript.h>
#include <Script/cppscript.hpp>
#include <imgui/imgui.h>
#include "Core/FilePath.h"
#include "Resource/ResourcesManager.h"
#include <FML/Core/GameObject.h>
using namespace gui;

void ScriptManagerInspector::Draw(bool *value, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CScriptManager& inTarget = inGameObject->get<fmc::CScriptManager>();

    if(ImGui::CollapsingHeader("ScriptManagerInspector", value))
    {
		ImGui::Indent(10.0f);

		std::vector<std::string> scriptsToDelete;

		fmc::LuaScripts &&scripts = inTarget.GetLuaScripts();
		for (auto &&script : scripts)
		{
			std::string scriptName = script->GetScriptName() + "##" + std::to_string(inGameObject->GetID());
			bool toKeep = true;
			if (ImGui::CollapsingHeader(scriptName.c_str(), &toKeep))
			{

				std::map<std::string, fm::LuaScript::ScriptArgument> valuesToStartWith = script->GetListValues();
				for (auto && valueToStartWith : valuesToStartWith)
				{
					std::string name = valueToStartWith.first;
					ImGui::Text("%s", name.c_str());

					ImGui::SameLine();

					name = "##" + std::to_string(inGameObject->GetID()) + "value" + valueToStartWith.first;

					bool hasChanged = false;
					std::any argValue = valueToStartWith.second.value;
					if (valueToStartWith.second.typeKind == fm::LuaScript::ScriptTypeValue::NUMERAL)
					{
						if (script->HasStarted())
						{
							script->EvaluateVariable(valueToStartWith.second, argValue);
						}
						float v = std::any_cast<float>(argValue);
						hasChanged = ImGui::InputFloat(name.c_str(), &v, 0.01, 1);
						if (hasChanged)
						{
							argValue = v;
						}
					}

					if (hasChanged)
					{

						if (script->HasStarted())
						{
							//TODO event
						}
						else
						{
							script->SetStartValue(valueToStartWith.first, argValue);
						}
					}
				}
			}
			if (ImGui::Button("Reload"))
			{
				inTarget.ReloadScript(script->GetScriptName());
			}
			if (!toKeep)
			{
				scriptsToDelete.emplace_back(script->GetScriptName());
			}
		}
		ImGui::Unindent(10.0f);

		static char nameScript[128] = "ClassName";

		ImGui::InputText("Class Name", nameScript, IM_ARRAYSIZE(nameScript));
        if(ImGui::Button("Add Script"))
        {

			fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::USER_LUA_LOCATION);
			if (p.GetPath().empty())
			{
				p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_RESOURCES_LOCATION);
			}
			std::string fileName = std::string(nameScript) + ".lua";
			p.ToSub(fileName);
			
			inTarget.addScriptLua(p);
			
        }

        if(!scriptsToDelete.empty())
        {
            for(auto &s : scriptsToDelete)
            {
				inTarget.RemoveScript(s);
            }
        }


    }
}

void ScriptManagerInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CScriptManager>();
}