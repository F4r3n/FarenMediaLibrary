#include <Engine.h>
#include <Window.h>
#include <TimeDef.h>
#include "Core/application.h"
#include <nlohmann/json.hpp>
#include "Core/SceneManager.h"
#include <iomanip>      // std::setw
#include "Core/Scene.h"
#include <fstream>
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
}

Application::Application()
{

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
}

void Application::Stop()
{
	fm::SceneManager::get().ClearScene(_nameLastScene + "_", true);
	fm::SceneManager::get().setCurrentScene(_nameLastScene, false);
	fm::SceneManager::get().getCurrentScene()->ResetStatusGo();
	_engine->Stop();
	//TODO garbage collect
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
    _window = new fm::Window(_currentConfig.width, _currentConfig.height, _currentConfig.name);
    _window->Init();
    _engine->Init();
}

void Application::Update(bool withEditor)
{
    _window->update(_currentConfig.fpsWanted, !withEditor);
    _engine->Update(fm::Time::dt);
}


void Application::DeInit()
{
    delete _window;
    delete _engine;
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






