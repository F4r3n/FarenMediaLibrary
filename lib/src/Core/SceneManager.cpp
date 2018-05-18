#include "Core/SceneManager.h"
using namespace fm;
SceneManager SceneManager::_instance;
SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::setCurrentScene(const std::string &name) {
    if(_scenes.find(name) != _scenes.end()) {
        _currentScene = _scenes[name];
    }
}

SceneManager& SceneManager::get() {
    return _instance;
}
void SceneManager::Serialize(nlohmann::json &outjson)
{
    for(auto &scene : _scenes)
    {
        nlohmann::json s;
        scene.second->Serialize(s);
        outjson[scene.first] = s;
    }
}


void SceneManager::addScene(Scene *scene)
{
    _scenes.insert(std::pair<std::string, Scene*>(scene->getName(), scene));    
}

Scene* SceneManager::getScene(const std::string &name)
{
    if(_scenes.find(name) != _scenes.end())
    {
        return _scenes[name];
    }
    return nullptr;
}
