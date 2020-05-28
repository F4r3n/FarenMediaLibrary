#include "inspector/textInspector.hpp"
#include "Components/CText.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Text, fmc::CText)

void TextInspector::_Init()
{
}

void TextInspector::_DeInit()
{
}

void TextInspector::Draw(bool *)
{
	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());
	if(ImGui::CollapsingHeader(name.c_str()))
    {
		static char text[1024] = "\0";
		name = "text" + std::to_string(_target->GetIDEntity());
		ImGui::InputTextMultiline(name.c_str(), text, sizeof(text));
		std::string content(text);

		_target->SetText(content);
    }
}


