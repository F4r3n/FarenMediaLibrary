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
#include "GL/glew.h"
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
	return Serialize();
}

std::shared_ptr<fm::Scene> Application::CreateNewScene(const std::string &inNewSceneName)
{
	_sceneManager->AddNewScene("newScene");
	_sceneManager->SetCurrentScene("newScene", false);
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
    nlohmann::json j;
    i >> j;
	_sceneManager->Read(j);
    return true;
}

void Application::Start(bool inSandbox)
{

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_START));

	if (inSandbox)
	{
		nlohmann::json save;
		_sceneManager->SerializeCurrentScene(save);

		std::shared_ptr<fm::Scene> current = _sceneManager->GetCurrentScene();
		_nameLastScene = current->getName();
		std::string privateName = _nameLastScene + "_";
		std::shared_ptr<fm::Scene> s = _sceneManager->AddPrivateScene(privateName);
		s->Read(save);
		
		current->SetStatusToGo(false);

		_sceneManager->SetCurrentScene(privateName, true);
	}
	
	_engine->Start();

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_START));

}

void Application::Stop()
{
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_STOP));


	_sceneManager->ClearScene(_nameLastScene + "_", true);
	_sceneManager->SetCurrentScene(_nameLastScene, false);
	_sceneManager->GetCurrentScene()->ResetStatusGo();
	_engine->Stop();

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_STOP));

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


void Application::LoadProject(const fm::FilePath& inFilePath)
{
	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_PRE_LOAD));

	_sceneManager->ClearAll(false);
	
	SetProjectName(inFilePath.GetName(true));
	SetUserDirectory(inFilePath.GetParent());

	Read();

	NotifyAll(EventObserver((size_t)fm::Application::Event::ON_AFTER_LOAD));
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


void Application::SetUserDirectory(const fm::Folder &inPath)
{
	_currentConfig.userDirectory = inPath;
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
	return _sceneManager->GetCurrentScene()->getName();
}

std::shared_ptr<fm::Scene> Application::GetCurrentScene() const
{
	return _sceneManager->GetCurrentScene();
}

void Application::RegisterCurrentConfig()
{
	bool found = false;
	for (size_t i = 0; i < _lastConfigsUsed.Size(); ++i)
	{
		if ((_lastConfigsUsed[i].name == _currentConfig.name) && (_lastConfigsUsed[i].userDirectory.GetPath() == _currentConfig.userDirectory.GetPath()))
		{
			found = true;
			break;
		}
	}

	_lastConfigsUsed.Push(_currentConfig);

}

void Application::GetLastConfigs(std::vector<fm::Config> &outConfig)
{
	for (size_t i = 0; i < _lastConfigsUsed.Size(); ++i)
	{
		outConfig.push_back(_lastConfigsUsed[i]);
	}
}

bool Application::IsRunning() const
{
	return _engine->GetStatus() == SYSTEM_MANAGER_MODE::RUNNING;
}







