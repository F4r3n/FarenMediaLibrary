#include "inspector/textInspector.hpp"
#include "Components/CText.h"
#include <imgui/imgui.h>
#include <FML/Core/GameObject.h>
#include <FML/Components/CText.h>
using namespace gui;


void TextInspector::Draw(bool *, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CText& inTarget = inGameObject->get<fmc::CText>();

	std::string name = inTarget.GetName() + "##" + std::to_string(inGameObject->GetID());
	if(ImGui::CollapsingHeader(name.c_str()))
    {
		static char text[1024] = "\0";
		name = "text" + std::to_string(inGameObject->GetID());
		ImGui::InputTextMultiline(name.c_str(), text, sizeof(text));
		std::string content(text);

		inTarget.SetText(content);
    }
}


void TextInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CText>();
}