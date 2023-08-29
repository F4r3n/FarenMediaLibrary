#include "Editor.h"
#include "Core/application.h"
#include "nlohmann/json.hpp"
#include "Resource/ResourcesManager.h"
#include "Core/Scene.h"
#include <fstream>
#include <iomanip>
using namespace fm;

Editor::Editor() : fm::Observable("Editor")
{

}

std::shared_ptr<fm::Scene> Editor::CreateNewScene(const fm::FilePath& inScenePath)
{
	std::shared_ptr<fm::Scene> scene = fm::Application::Get().AddNewScene(inScenePath);
	NotifyAll(EventObserver((size_t)Editor::Event::ON_PRE_SCENE_LOAD));
	fm::Application::Get().SetCurrentScene(scene->GetName(), false);
	NotifyAll(EventObserver((size_t)Editor::Event::ON_AFTER_SCENE_LOAD));

	return fm::Application::Get().GetCurrentScene();
}

std::shared_ptr<fm::Scene> Editor::CreateEditorScene()
{
	return fm::Application::Get().AddPrivateScene("_editor");
}

void Editor::Start()
{

	NotifyAll(EventObserver((size_t)Editor::Event::ON_PRE_START));


	NotifyAll(EventObserver((size_t)Editor::Event::ON_AFTER_START,
		(std::function<void(void)>)[this]() {
			std::shared_ptr<fm::Scene> current = fm::Application::Get().GetCurrentScene();
			nlohmann::json save;
			fm::Application::Get().SerializeCurrentScene(save);

			_nameLastScene = current->GetName();
			std::string privateName = _nameLastScene + "_";
			std::shared_ptr<fm::Scene> s = fm::Application::Get().AddPrivateScene(privateName);
			s->Read(save);

			current->SetStatusToGo(false);

			fm::Application::Get().SetCurrentScene(privateName, true);
			fm::Application::Get().Start();

		})
	);
	


}


void Editor::Stop()
{

	NotifyAll(EventObserver((size_t)Editor::Event::ON_PRE_STOP,
		(std::function<void(void)>)([this]() {
			fm::Application::Get().ClearScene(_nameLastScene + "_", true);
			}
			))
	);

	NotifyAll(EventObserver((size_t)Editor::Event::ON_AFTER_STOP,
		(std::function<void(void)>)[this]() {
			fm::Application::Get().SetCurrentScene(_nameLastScene, false);
			fm::Application::Get().GetCurrentScene()->ResetStatusGo();
			fm::Application::Get().Stop();
		}
	)
	);

}

void Editor::NewProject(const fm::Folder& inPath)
{

	_SetUserDirectory(inPath);

	NotifyAll(EventObserver((size_t)Editor::Event::ON_PRE_LOAD));
	std::function<void(void)>&& f =
		[inPath]() {
		fm::Application::Get().Read();
		if (fm::Application::Get().GetCurrentScene() == nullptr) //Empty file, create new default scene
		{
			fm::FilePath path(fm::LOCATION::USER_LOCATION, "NewScene");
			std::shared_ptr<fm::Scene> scene = fm::Application::Get().AddNewScene(path);
			fm::Application::Get().SetCurrentScene(scene->GetName(), false);
		}
		else
		{
			fm::Application::Get().GetCurrentScene()->Load();
		}
	};
	NotifyAll(EventObserver((size_t)Editor::Event::ON_AFTER_LOAD, std::move(f)));


}

void Editor::_SetUserDirectory(const fm::Folder& inPath)
{
	fm::Application::Get().SetUserDirectory(inPath);
	_SaveLastProjectOpened(inPath.GetPath());
}


void Editor::SetCurrentScene(const std::string& inName)
{
	NotifyAll(EventObserver((size_t)Editor::Event::ON_PRE_SCENE_LOAD));
	fm::Application::Get().SetCurrentScene(inName, false);
	NotifyAll(EventObserver((size_t)Editor::Event::ON_AFTER_SCENE_LOAD));
}

void Editor::GetLastProjectsOpened(std::vector<fm::FilePath>& outPath) const
{
	File file(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::SETTINGS_LAST_PROJECTS));
	std::string&& content = file.GetContent();

	if (!content.empty())
	{
		nlohmann::json json = nlohmann::json::parse(content);

		for (auto it = json.begin(); it != json.end(); ++it)
		{
			outPath.emplace_back(std::string(*it));
		}
	}
}

void Editor::_SaveLastProjectOpened(const fm::FilePath& inFilePath)
{
	Folder settingsFolder(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::SETTINGS));
	if (!settingsFolder.Exist())
	{
		settingsFolder.CreateFolder();
	}

	const size_t limitToSave = 10;
	File file(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::SETTINGS_LAST_PROJECTS));
	std::string&& content = file.GetContent();

	nlohmann::json jarray;
	jarray.push_back(inFilePath.GetPathString());

	if (!content.empty())
	{
		nlohmann::json json = nlohmann::json::parse(content);

		size_t i = 1;
		for (auto it = json.begin(); it != json.end(); ++it)
		{
			if (inFilePath.GetPathString() != *it)
			{
				jarray.push_back(*it);
				i++;
			}

			if (i >= limitToSave)
				break;
		}
	}

	std::ofstream o(file.GetPath().GetPath(), std::ofstream::out);
	o << std::setw(4) << jarray << std::endl;
	o.close();
}

std::shared_ptr<fm::Scene>	Editor::RenameScene(std::shared_ptr<fm::Scene> inCurrentScene, const fm::FilePath& inPath)
{
	bool isSameScene = fm::Application::Get().GetCurrentScene()->GetName() == inCurrentScene->GetName();

	auto s = fm::Application::Get().RenameScene(inCurrentScene, inPath);
	if (isSameScene)
	{
		SetCurrentScene(s->GetName());
	}
	return s;
}

std::shared_ptr<fm::Scene>	Editor::LoadScene(const fm::FilePath& inPath)
{
	auto s = fm::Application::Get().GetScene(inPath.GetName(true));
	if (s != nullptr)
	{
		s->Load();
	}
	else
	{
		s = fm::Application::Get().AddNewScene(inPath);
	}

	return s;
}

std::shared_ptr<fm::Scene> Editor::GetCurrentScene() const
{
	return fm::Application::Get().GetCurrentScene();
}

const std::string& Editor::GetCurrentSceneName() const
{
	return fm::Application::Get().GetCurrentScene()->GetName();
}

std::shared_ptr<fm::Scene> Editor::GetScene(const std::string& inName) const
{
	return fm::Application::Get().GetScene(inName);
}

void Editor::SerializeCurrentScene()
{
	fm::Application::Get().SerializeCurrentScene();
}