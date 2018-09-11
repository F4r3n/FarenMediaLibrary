#include <Engine.h>
#include <Window.h>
#include <Time.h>
#include "Core/application.h"
#include <nlohmann/json.hpp>
#include "Core/SceneManager.h"

#include <fstream>
using namespace fm;

const std::string fileOutput = "fml.conf";

Application::Application(const Config &inConfig)
{
    _currentConfig = inConfig;
}

Application::~Application()
{

}

bool Application::Serialize()
{
    nlohmann::json s;
    SceneManager::get().Serialize(s);

    std::ofstream o(fileOutput, std::ofstream::out);
    o << std::setw(4) << s << std::endl;
    o.close();
    return true;

}

bool Application::Read()
{
    nlohmann::json s;

    std::ifstream i(fileOutput);
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
