#include "GLauncher.h"
#include "PortableFileDialog.h"
#include "Editor.h"
using namespace gui;
GLauncher::GLauncher() : GWindow("Launcher", false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoDocking)
{
	_kind = gui::WINDOWS::WIN_LAUNCHER;
	//SetModal(true);
}

GLauncher::~GLauncher()
{
}

void GLauncher::OnInit()
{
	const size_t width = 400;
	const size_t height = 200;
	ImGuiViewport* viewPort = ImGui::GetMainViewport();
	SetPosition(ImVec2(viewPort->Pos.x + viewPort->Size.x/2 - width/2, viewPort->Pos.y + viewPort->Size.y/2 - height/2));
	SetSize(ImVec2(width, height));

	if (_listProjects.empty())
	{
		Editor::Get().GetLastProjectsOpened(_listProjects);
		_projectSelected = -1;
	}
}


void GLauncher::_DisplayWindow_Create_Project()
{
	pfd::select_folder dialog = pfd::select_folder("Create to...", ".");
	std::string&& resultFromDialog = dialog.result();

	if (!resultFromDialog.empty())
	{
		_result = fm::FilePath(resultFromDialog);
		_enabled = false;
	}
}


void GLauncher::CustomDraw()
{
	ImGui::BeginGroup();
	if (ImGui::Button("Create/Load Project"))
	{
		_DisplayWindow_Create_Project();
	}

	ImGui::EndGroup();

	ImGui::SameLine();

	if (ImGui::BeginListBox("##List"))
	{
		size_t i = 0;
		for (auto && path : _listProjects)
		{
			if (ImGui::Selectable(path.GetPathString().c_str(), _projectSelected == i, ImGuiSelectableFlags_AllowDoubleClick))
			{
				_projectSelected = i;

				_result = _listProjects.at(_projectSelected);
				_enabled = false;
				break;
				
			}
			i++;
		}
		ImGui::EndListBox();
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


