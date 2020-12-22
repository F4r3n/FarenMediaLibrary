#include "GLauncher.h"

using namespace gui;
GLauncher::GLauncher() : GWindow("Launcher", true, ImGuiWindowFlags_HorizontalScrollbar)
{
	_kind = gui::WINDOWS::WIN_LAUNCHER;
}

GLauncher::~GLauncher()
{
}

void GLauncher::OnInit()
{
	ImGuiViewport* viewPort = ImGui::GetMainViewport();
	SetPosition(ImVec2(viewPort->Pos.x, viewPort->Pos.y));
	SetSize(ImVec2(viewPort->Size.x, viewPort->Size.y));
}


void GLauncher::CustomDraw()
{
	if (ImGui::Button("Create Project"))
	{

	}
	if (ImGui::Button("Load Project"))
	{

	}
}

void GLauncher::BeforeWindowCreation()
{
}

void GLauncher::AfterWindowCreation()
{
}


void GLauncher::_Update(float dt, Context &inContext)
{
	
}


