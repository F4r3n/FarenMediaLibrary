#include "ToolBar.hpp"

using namespace gui;

ToolBar::ToolBar() : GWindow("ToolBar", true, ImGuiWindowFlags_NoDecoration)
{
	_enabled = true;
}

ToolBar::~ToolBar()
{

}

void ToolBar::Update(float, Context &inContext)
{

}
void ToolBar::CustomDraw()
{
	//ImGui::same
}