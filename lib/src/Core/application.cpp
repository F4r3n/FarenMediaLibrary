#include "Core/application.h"
#include "Core/SceneManager.h"
#include <json.hpp>
using namespace fm;

Application::Application()
{

}

Application::~Application()
{

}

bool Application::Serialize()
{
    nlohmann::json s;
    SceneManager::get().Serialize(s);
    std::string result = s.dump();
    std::cout << result << std::endl;

}

bool Application::Read()
{

}

void Application::Start()
{
}

void Application::Init()
{

}
