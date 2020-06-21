#include "ProjectSettingsWindow.h"
#include "Core/application.h"

#include <imgui/imgui_internal.h>

using namespace gui;

ProjectSettingsWindow::ProjectSettingsWindow() : GWindow("Project Settings", false, ImGuiWindowFlags_AlwaysAutoResize)
{
	_enabled = false;
}

ProjectSettingsWindow::~ProjectSettingsWindow()
{

}


void ProjectSettingsWindow::CustomDraw()
{
	//ImGui::Text("%s", _projectSettings.name.c_str());

	//static char bufferName[256];
	//ImGui::InputText("Poject name", bufferName, 256);
	//if (ImGui::Button("Valid"))
	//{
	//	_projectSettings.name = std::string(bufferName);
	//	ImGui::CloseCurrentPopup();
	//}
}

void ProjectSettingsWindow::WillClose()
{

}