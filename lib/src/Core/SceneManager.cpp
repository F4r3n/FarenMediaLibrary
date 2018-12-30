#include <nlohmann/json.hpp>

#include "Core/Scene.h"

#include "Core/SceneManager.h"
#include "Core/GameObject.h"
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
        outjson[scene.second->getName()] =  s;
    }
}


void SceneManager::SerializeEditor(nlohmann::json &outjson)
{
	nlohmann::json s;
	_editorScene->Serialize(s);
	outjson = s;
}


bool SceneManager::ReadEditor(const nlohmann::json &injson)
{
	_editorScene->Read(injson);	
	return true;
}

bool SceneManager::Read(const nlohmann::json &injson)
{
    for (nlohmann::json::const_iterator it = injson.cbegin(); it != injson.cend(); ++it) {
        Scene *s = new Scene(it.key());
        nlohmann::json o = it.value();
        s->Read(o);

        addScene(s);
    }
    return true;
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

void SceneManager::InitEditorScene()
{
	if (_editorScene != nullptr)
	{
		delete _editorScene;
	}

	_editorScene = new Scene("Editor");

}

