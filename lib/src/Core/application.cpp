#include <Engine.h>
#include <Window.h>
#include <TimeDef.h>
#include "Core/application.h"
#include <nlohmann/json.hpp>
#include "Core/SceneManager.h"
#include <iomanip>      // std::setw
#include "Core/Scene.h"
#include <fstream>
#include "Resource/ResourcesManager.h"
#include "SystemManager.h"
#include "Core/Debug.h"
#include <set>
using namespace fm;

const std::string PROJECT_FILE_NAME_EXTENSION = ".fml";
const std::string EDITOR_FILE_NAME_EXTENSION = ".editor.fml";
const std::string TEMPORARY_SCENE_NAME = "_temp";

Application::~Application()
{
}


void Application::SetConfig(const Config &inConfig)
{
	_currentConfig = inConfig;

	if (!_currentConfig.userDirectory.GetPath().IsValid())
	{
		_currentConfig.userDirectory = Folder(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::WORKING_DIRECTORY));
	}
}


Application::Application() : fm::Observable("Application")
{
	_sceneManager = std::make_unique<fm::SceneManager>();
}


bool Application::SerializeCurrentScene() const
{
	_sceneManager->GetCurrentScene()->Save();
	return Serialize();
}


std::shared_ptr<fm::Scene> Application::CreateNewScene(const fm::FilePath& inScenePath)
{
	std::shared_ptr<fm::Scene> scene = _sceneManager->AddNewScene(inScenePath);
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_SCENE_LOAD));
	_sceneManager->SetCurrentScene(scene->GetName(), false);
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_SCENE_LOAD));

	return _sceneManager->GetCurrentScene();
}

std::shared_ptr<fm::Scene> Application::CreateEditorScene()
{
	return _sceneManager->AddPrivateScene("_editor");
}


bool Application::Serialize() const
{
	
	nlohmann::json s;
	_sceneManager->Serialize(s);

	File f(_currentConfig.userDirectory, _currentConfig.name + PROJECT_FILE_NAME_EXTENSION);
	
	std::ofstream o(f.GetPath().GetPath(), std::ofstream::out);
	o << std::setw(4) << s << std::endl;
	o.close();
	
    return true;

}


bool Application::Read()
{
    nlohmann::json s;
	File f(_currentConfig.userDirectory, _currentConfig.name + PROJECT_FILE_NAME_EXTENSION);

    std::ifstream i(f.GetPath().GetPath());
	try
	{
		nlohmann::json j;
		i >> j;
		_sceneManager->Read(j);
	}
	catch (std::exception e)
	{
		return false;
	}
    return true;
}


void Application::Start(bool inSandbox)
{
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_START));


	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_START,
		(std::function<void(void)>)[this]() {
			std::shared_ptr<fm::Scene> current = _sceneManager->GetCurrentScene();
			nlohmann::json save;
			_sceneManager->SerializeCurrentScene(save);

			_nameLastScene = current->GetName();
			std::string privateName = _nameLastScene + "_";
			std::shared_ptr<fm::Scene> s = _sceneManager->AddPrivateScene(privateName);
			s->Read(save);

			current->SetStatusToGo(false);

			_sceneManager->SetCurrentScene(privateName, true);
			_engine->Start();

	})
	);

}


void Application::Stop()
{
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_STOP,
		(std::function<void(void)>)([this]() {
			_sceneManager->ClearScene(_nameLastScene + "_", true);
		}
		))
	);

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_STOP,
		(std::function<void(void)>)[this]() {
			_sceneManager->SetCurrentScene(_nameLastScene, false);
			_sceneManager->GetCurrentScene()->ResetStatusGo();
			_engine->Stop();
		}
	)
	);

}


fm::Window* Application::GetWindow() const
{
    return _window;
}
fm::Engine* Application::GetEngine() const
{
    return _engine;
}

void Application::Init()
{
    _engine = new fm::Engine();
    _window = new fm::Window(_currentConfig.width, _currentConfig.height, _currentConfig.windowFlag);
    _window->Init();

	_window->setName(_currentConfig.name);

	fm::ResourcesManager::get().LoadShaders();
	fm::ResourcesManager::get().LoadFonts();
	fm::ResourcesManager::get().LoadMaterials();

    _engine->Init();
}


void Application::Update(bool withEditor)
{
    _window->update(_currentConfig.fpsWanted);
    _engine->Update(fm::Time::dt);
}


void Application::DeInit()
{
    delete _engine;
	delete _window;
}

void Application::NewProject(const fm::Folder& inPath)
{
	_SetUserDirectory(inPath);

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_LOAD));
	std::function<void(void)> &&f =
		[this, inPath]() {
		Read();
		if (_sceneManager->GetCurrentScene() == nullptr) //Empty file, create new default scene
		{
			fm::FilePath path(fm::LOCATION::USER_LOCATION, "NewScene");
			std::shared_ptr<fm::Scene> scene = _sceneManager->AddNewScene(path);
			_sceneManager->SetCurrentScene(scene->GetName(), false);
		}
		else
		{
			_sceneManager->GetCurrentScene()->Load();
		}
	};
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_LOAD, std::move(f)));

}


void Application::_SetUserDirectory(const fm::Folder &inPath)
{
	_currentConfig.userDirectory = inPath;
	_SaveLastProjectOpened(inPath.GetPath());
}


const fm::Folder& Application::GetUserDirectory() const
{
	return _currentConfig.userDirectory;
}


void Application::SetProjectName(const std::string &inName)
{
	_currentConfig.name = inName;
}


const fm::Config& Application::GetCurrentConfig() const
{
	return _currentConfig;
}


std::shared_ptr<fm::Scene> Application::GetScene(const std::string &inName) const
{
	return _sceneManager->GetScene(inName);
}


const std::string& Application::GetCurrentSceneName() const
{
	return _sceneManager->GetCurrentScene()->GetName();
}


std::shared_ptr<fm::Scene> Application::GetCurrentScene() const
{
	return _sceneManager->GetCurrentScene();
}


bool Application::IsRunning() const
{
	return _engine->GetStatus() == SYSTEM_MANAGER_MODE::RUNNING;
}

std::shared_ptr<fm::Scene>	Application::RenameScene(std::shared_ptr<fm::Scene> inCurrentScene, const fm::FilePath& inPath)
{
	bool isSameScene = _sceneManager->GetCurrentScene()->GetName() == inCurrentScene->GetName();

	auto s = _sceneManager->RenameScene(inCurrentScene, inPath);
	if (isSameScene)
	{
		SetCurrentScene(s->GetName());
	}
	return s;
}


std::shared_ptr<fm::Scene> Application::LoadScene(const fm::FilePath& inPath)
{
	auto s = _sceneManager->GetScene(inPath.GetName(true));
	if (s != nullptr)
	{
		s->Load();
	}
	else
	{
		s = _sceneManager->AddNewScene(inPath);
	}

	return s;
}


void Application::SetCurrentScene(const std::string& inName)
{
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_SCENE_LOAD));
	_sceneManager->SetCurrentScene(inName, false);
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_SCENE_LOAD));
}

void Application::GetLastProjectsOpened(std::vector<fm::FilePath>& outPath) const
{
	File file(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::SETTINGS_LAST_PROJECTS));
	std::string&& content = file.GetContent();

	if (!content.empty())
	{
		nlohmann::json json = nlohmann::json::parse(content);

		for (auto it = json.begin(); it != json.end(); ++it)
		{
			outPath.emplace_back(*it);
		}
	}
}

void Application::_SaveLastProjectOpened(const fm::FilePath& inFilePath)
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
	jarray.push_back(inFilePath.GetPath());

	if (!content.empty())
	{
		nlohmann::json json = nlohmann::json::parse(content);

		size_t i = 1;
		for (auto it = json.begin(); it != json.end(); ++it)
		{
			if (inFilePath.GetPath() != *it)
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



