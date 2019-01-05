#include "SaveProjectWindow.h"
#include "Core/application.h"
#include "dialogfilebrowser.h"
using namespace gui;

SaveProjectWindow::SaveProjectWindow(const std::string &inName) : GWindow(inName)
{
	_isInit = false;
	_fileBrowserOpened = true;
}


void SaveProjectWindow::WillClose()
{
	_isInit = false;
}


void SaveProjectWindow::CustomDraw()
{
	if (!_isInit)
	{
		fm::Config c = fm::Application::Get().GetCurrentConfig();
		_projectSettings.name = c.name;
		_projectSettings.path = c.userDirectory;
		_fileBrowserOpened = true;
	}

	ImGui::Text("%s", _projectSettings.name.c_str());

	static char bufferName[256];
	strcpy(bufferName, _projectSettings.name.c_str());
	ImGui::InputText("Poject name", bufferName, 256);

	static char path[256];
	ImGui::InputText("##Path", path, 256);

	ImGui::SameLine();
	if (ImGui::Button("Location") || DialogFileBrowser::Get().IsVisible())
	{
		DialogFileBrowser::Get().Import(fm::Application::Get().GetUserDirectory().GetPath().empty() ? "." : fm::Application::Get().GetUserDirectory().GetPath(),
			"Save", &_fileBrowserOpened);

		if (DialogFileBrowser::Get().IsValid())
		{
			fm::FilePath result = DialogFileBrowser::Get().GetResult();
			if (result.IsFolder())
			{
				fm::FilePath r(result);
				r.Append(_projectSettings.name);
				_projectSettings.path = r;
				fm::Application::Get().SetUserDirectory(r);


				r.Append("Resources");
				_projectSettings.resourcesFolder = r.GetPath();

			}

			strcpy(path, _projectSettings.path.GetPath().c_str());
		}
	}
	else
	{
		_fileBrowserOpened = true;
	}

	if (ImGui::Button("Valid"))
	{
		_projectSettings.name = std::string(bufferName);
		//_windowStates[WIN_CREATE_PROJECT] = false;

		_projectSettings.path.CreateFolder();
		CreateFolder(_projectSettings.resourcesFolder.c_str());
		fm::Application::Get().SetProjectName(_projectSettings.name);
		fm::Application::Get().Serialize(true);

		ImGui::CloseCurrentPopup();
	}
}