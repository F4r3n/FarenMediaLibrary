#include "Core/application.h"
#include "Core/SceneManager.h"
#include <json.hpp>
#include <fstream>
using namespace fm;

const std::string fileOutput = "fml.conf";

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

void Application::Init()
{

}
