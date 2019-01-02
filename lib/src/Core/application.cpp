#include <Engine.h>
#include <Window.h>
#include <TimeDef.h>
#include "Core/application.h"
#include <nlohmann/json.hpp>
#include "Core/SceneManager.h"
#include <iomanip>      // std::setw

#include <fstream>
using namespace fm;

const std::string PROJECT_FILE_NAME_EXTENSION = ".fml";
const std::string EDITOR_FILE_NAME_EXTENSION = ".editor.fml";
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


bool Application::Serialize(bool serializeEditor)
{
	{
		nlohmann::json s;
		SceneManager::get().Serialize(s);

		std::ofstream o(_currentConfig.userDirectory.GetPath() + _currentConfig.name + PROJECT_FILE_NAME_EXTENSION, std::ofstream::out);
		o << std::setw(4) << s << std::endl;
		o.close();
	}

	if(serializeEditor)
	{
		nlohmann::json s;
		SceneManager::get().SerializeEditor(s);

		std::ofstream o(_currentConfig.userDirectory.GetPath() + _currentConfig.name + EDITOR_FILE_NAME_EXTENSION, std::ofstream::out);
		o << std::setw(4) << s << std::endl;
		o.close();
	}
    return true;

}

bool Application::Read()
{
    nlohmann::json s;

    std::ifstream i(_currentConfig.userDirectory.GetPath());
    nlohmann::json j;
    i >> j;
    SceneManager::get().Read(j);
    return true;
}

void Application::Start()
{
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

void Application::Update()
{
    _window->update(_currentConfig.fpsWanted, false);
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

const fm::FilePath& Application::GetUserDirectory()
{
	return _currentConfig.userDirectory;
}

void Application::SetProjectName(const std::string &inName)
{
	_currentConfig.name = inName;
}



