#include "Core/application.h"


#include <Engine.h>
#include <Window.h>
#include <TimeDef.h>
#include <nlohmann/json.hpp>
#include "Core/SceneManager.h"
#include <iomanip>      // std::setw
#include "Core/Scene.h"
#include <fstream>
#include "Resource/ResourcesManager.h"
#include "SystemManager.h"
#include "Core/Debug.h"
#include "Core/SceneManager.h"
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


Application::Application() 
{
	_sceneManager = std::make_unique<fm::SceneManager>();
}


bool Application::SerializeCurrentScene() const
{
	_sceneManager->GetCurrentScene()->Save();
	return Serialize();
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

void Application::Start()
{
	_engine->Start();
}

void Application::Stop()
{
	_engine->Stop();
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
    _window = new fm::Window(_currentConfig.width, _currentConfig.height, _currentConfig.graphicAPI, _currentConfig.windowFlag);
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



void Application::LoadProject(const fm::Folder& inPath)
{
	_currentConfig.userDirectory = inPath;
	if (Read())
	{
		_sceneManager->GetCurrentScene()->Load();
	}
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



std::shared_ptr<fm::Scene>	Application::AddNewScene(const fm::FilePath& inPath)
{
	return _sceneManager->AddNewScene(inPath);
}


void Application::SetCurrentScene(const std::string& inName, bool isPrivate)
{
	return _sceneManager->SetCurrentScene(inName, isPrivate);
}


std::shared_ptr<fm::Scene>	Application::AddPrivateScene(const std::string& inName)
{
	return _sceneManager->AddPrivateScene(inName);
}


bool Application::ClearScene(const std::string& inName, bool isPrivate, bool remove)
{
	return _sceneManager->ClearScene(inName, isPrivate, remove);
}


std::shared_ptr<Scene>	Application::RenameScene(std::shared_ptr<Scene> inCurrentScene, const fm::FilePath& inPath)
{
	return _sceneManager->RenameScene(inCurrentScene, inPath);
}


void Application::SerializeCurrentScene(nlohmann::json& outjson)
{
	return _sceneManager->SerializeCurrentScene(outjson);
}
