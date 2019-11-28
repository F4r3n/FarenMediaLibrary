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
using namespace fm;

const std::string PROJECT_FILE_NAME_EXTENSION = ".fml";
const std::string EDITOR_FILE_NAME_EXTENSION = ".editor.fml";
const std::string TEMPORARY_SCENE_NAME = "_temp";

Application::~Application()
{
	_observers.clear();
}

void Application::SetConfig(const Config &inConfig)
{
	_currentConfig = inConfig;
}

Application::Application()
{

}


void Application::AddApplicationObserver(std::shared_ptr<ApplicationObserver> inObserver)
{
	_observers.emplace_back(inObserver);
}


bool Application::Serialize(std::shared_ptr<fm::Scene> editorScene)
{
	{
		nlohmann::json s;
		SceneManager::get().Serialize(s);

		FilePath p(_currentConfig.userDirectory);
		p.Append(_currentConfig.name + PROJECT_FILE_NAME_EXTENSION);
		
		std::ofstream o(p.GetPath(), std::ofstream::out);
		o << std::setw(4) << s << std::endl;
		o.close();
	}

	if(editorScene != nullptr)
	{
		nlohmann::json s;
		editorScene->Serialize(s);

		FilePath p(_currentConfig.userDirectory);
		p.Append(_currentConfig.name + EDITOR_FILE_NAME_EXTENSION);
		std::ofstream o(p.GetPath(), std::ofstream::out);
		o << std::setw(4) << s << std::endl;
		o.close();
	}
    return true;

}

bool Application::Read()
{
    nlohmann::json s;
	FilePath p(_currentConfig.userDirectory);
	p.Append(_currentConfig.name + PROJECT_FILE_NAME_EXTENSION);
    std::ifstream i(p.GetPath());
    nlohmann::json j;
    i >> j;
    SceneManager::get().Read(j);
    return true;
}

void Application::Start(bool inSandbox)
{
	for (auto && o : _observers)
	{
		o->OnPreStart();
	}


	if (inSandbox)
	{
		nlohmann::json save;
		fm::SceneManager::get().SerializeCurrentScene(save);

		std::shared_ptr<fm::Scene> current = fm::SceneManager::get().getCurrentScene();
		_nameLastScene = current->getName();
		std::string privateName = _nameLastScene + "_";
		std::shared_ptr<fm::Scene> s = fm::SceneManager::get().AddPrivateScene(privateName);
		s->Read(save);
		
		current->SetStatusToGo(false);

		fm::SceneManager::get().setCurrentScene(privateName, true);
	}
	
	_engine->Start();

	for (auto && o : _observers)
	{
		o->OnAfterStart();
	}
}

void Application::Stop()
{
	for (auto && o : _observers)
	{
		o->OnPreStop();
	}

	fm::SceneManager::get().ClearScene(_nameLastScene + "_", true);
	fm::SceneManager::get().setCurrentScene(_nameLastScene, false);
	fm::SceneManager::get().getCurrentScene()->ResetStatusGo();
	_engine->Stop();
	//TODO garbage collect

	for (auto && o : _observers)
	{
		o->OnAfterStop();
	}

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
	fm::ResourcesManager::get().CreateMaterials();

    _engine->Init();
}


void Application::LoadProject(const fm::FilePath& inFilePath)
{
	for (auto && o : _observers)
	{
		o->OnPreLoad();
	}
	fm::SceneManager::get().ClearAll(false);
	
	SetProjectName(inFilePath.GetName(true));
	SetUserDirectory(inFilePath.GetParent());
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

	Read();
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

	for (auto && o : _observers)
	{
		o->OnAfterLoad();
	}
}


void Application::Update(bool withEditor)
{
    _window->update(_currentConfig.fpsWanted, !withEditor);
    _engine->Update(fm::Time::dt);
}


void Application::DeInit()
{
    delete _engine;
	delete _window;

}


void Application::SetUserDirectory(const fm::FilePath &inPath)
{
	_currentConfig.userDirectory = inPath;
}

const fm::FilePath& Application::GetUserDirectory() const
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

void Application::RegisterCurrentConfig()
{
	bool found = false;
	for (size_t i = 0; i < _lastConfigsUsed.Size(); ++i)
	{
		if (_lastConfigsUsed[i].name == _currentConfig.name && _lastConfigsUsed[i].userDirectory.GetPath() == _currentConfig.userDirectory.GetPath())
		{
			found = true;
		}
	}

	_lastConfigsUsed.Push(_currentConfig);

}

void Application::GetLastConfigs(std::vector<fm::Config> &outConfig)
{
	//Berk
	for (size_t i = 0; i < _lastConfigsUsed.Size(); ++i)
	{
		outConfig.push_back(_lastConfigsUsed[i]);
	}
}

bool Application::IsRunning() const
{
	return _engine->GetStatus() == SYSTEM_MANAGER_MODE::RUNNING;
}







