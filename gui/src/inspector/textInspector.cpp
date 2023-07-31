#include "inspector/textInspector.hpp"
#include "Components/CText.h"
#include <imgui/imgui.h>
#include <ECS.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Text, fmc::CText)

void TextInspector::_Init()
{
}

void TextInspector::_DeInit()
{
}

void TextInspector::Draw(bool *, const Entity& inEntity)
{
	std::string name = _target->GetName() + "##" + std::to_string(inEntity.id().index());
	if(ImGui::CollapsingHeader(name.c_str()))
    {
		static char text[1024] = "\0";
		name = "text" + std::to_string(inEntity.id().index());
		ImGui::InputTextMultiline(name.c_str(), text, sizeof(text));
		std::string content(text);

		_target->SetText(content);
    }
}


void TextInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CText>(inEntity.id());
}