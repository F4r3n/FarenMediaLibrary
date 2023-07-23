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

bool DrawCombo(const char* inNameCombo, const std::vector<std::string> &inItems, int inCurrentIndex, int& outIndex)
{
	const char* item_current_char = inItems[inCurrentIndex].c_str();
	std::string nameCombo = inNameCombo;
	if (ImGui::BeginCombo(nameCombo.c_str(), item_current_char, 0))
	{
		for (size_t n = 0; n < inItems.size(); n++)
		{
			bool is_selected = (item_current_char == inItems[n]);
			if (ImGui::Selectable(inItems[n].c_str(), is_selected))
			{
				outIndex = n;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	return inCurrentIndex != outIndex;
}


void ScriptManagerInspector::_DeInit()
{
}


void ScriptManagerInspector::Draw(bool *value)
{
    if(ImGui::CollapsingHeader("ScriptManagerInspector", value))
    {
		ImGui::Indent(10.0f);

		std::vector<std::string> scriptsToDelete;

		fmc::LuaScripts &&scripts = _target->GetLuaScripts();
		for (auto &&script : scripts)
		{
			std::string scriptName = script->GetScriptName() + "##" + std::to_string(_target->GetIDEntity());
			bool toKeep = true;
			if (ImGui::CollapsingHeader(scriptName.c_str(), &toKeep))
			{

				std::map<std::string, fm::LuaScript::ScriptArgument> valuesToStartWith = script->GetListValues();
				for (auto && valueToStartWith : valuesToStartWith)
				{
					std::string name = valueToStartWith.first;
					ImGui::Text(name.c_str());

					ImGui::SameLine();

					name = "##" + std::to_string(_target->GetIDEntity()) + "value" + valueToStartWith.first;

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
				_target->ReloadScript(script->GetScriptName());
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
			p.ToSubFile(fileName);
			
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
