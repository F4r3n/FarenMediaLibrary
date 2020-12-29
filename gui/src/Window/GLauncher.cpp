#include "GLauncher.h"
#include "PortableFileDialog.h"
#include "Editor.h"
using namespace gui;
GLauncher::GLauncher() : GWindow("Launcher", false, ImGuiWindowFlags_HorizontalScrollbar)
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
		_result = fm::FilePath(resultFromDialog + fm::FilePath::GetFolderSeparator());
		_enabled = false;
	}
}


void GLauncher::CustomDraw()
{
	ImGui::BeginGroup();
	if (ImGui::Button("Create Project"))
	{
		_DisplayWindow_Create_Project();
	}
	if (ImGui::Button("Load Project"))
	{
		if (_projectSelected != -1)
		{
			try
			{
				_result = _listProjects.at(_projectSelected);
				_enabled = false;
			}
			catch (const std::exception &e)
			{
				_projectSelected = -1;
			}
		}
	}
	ImGui::EndGroup();

	ImGui::SameLine();

	if (ImGui::ListBoxHeader("##List"))
	{
		size_t i = 0;
		for (auto && path : _listProjects)
		{
			if (ImGui::Selectable(path.GetPath().c_str(), _projectSelected == i))
			{
				_projectSelected = i;
			}
			i++;
		}

		//To make it longer
		for (;i < 10; ++i)
		{
			ImGui::Selectable("", false);
		}

		ImGui::ListBoxFooter();
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


