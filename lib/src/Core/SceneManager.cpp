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
    if(_scenes.find(name) != _scenes.end()) 
	{
        _currentScene = _scenes[name];
    }
}

SceneManager& SceneManager::get() 
{
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


bool SceneManager::SerializePrivate(const std::string &inName, nlohmann::json &outjson)
{
	nlohmann::json json;
	for (auto& s : _privateScenes)
	{
		if (s->getName() == inName)
		{
			s->Serialize(json);
			outjson = json;
			return true;
		}
	}
	return false;
}


bool SceneManager::ReadPrivate(const std::string &inName, const nlohmann::json &injson)
{
	for (auto& s : _privateScenes)
	{
		if (s->getName() == inName)
		{
			s->Read(injson);
			return true;
		}
	}
	return false;
}

bool SceneManager::Read(const nlohmann::json &injson)
{
    for (nlohmann::json::const_iterator it = injson.cbegin(); it != injson.cend(); ++it) 
	{
        Scene *s = new Scene(it.key());
		_currentScene = s;
        nlohmann::json o = it.value();
        s->Read(o);

        addScene(s);
    }
    return true;
}



void SceneManager::addScene(Scene *scene)
{
    _scenes.insert(std::pair<std::string, Scene*>(scene->getName(), std::move(scene)));    
}

Scene* SceneManager::getScene(const std::string &name)
{
    if(_scenes.find(name) != _scenes.end())
    {
        return _scenes[name];
    }
    return nullptr;
}

std::shared_ptr<fm::Scene> SceneManager::AddPrivateScene(const std::string &inName)
{
	for (auto& s : _privateScenes)
	{
		if (s->getName() == inName)
		{
			return s;
		}
	}

	_privateScenes.emplace_back(std::make_shared<fm::Scene>(inName));
	return _privateScenes[_privateScenes.size() - 1];
}

void SceneManager::ClearAllPublic()
{

	for (auto &scene : _scenes)
	{
		scene.second->destroy();
	}
	_scenes.clear();
}



bool SceneManager::ClearScene(const std::string &inName)
{
	std::map<std::string, Scene*>::iterator it = _scenes.find(inName);
	if (it != _scenes.end())
	{
		it->second->destroy();
		return true;
	}

	for (auto& s : _privateScenes)
	{
		if (s->getName() == inName)
		{
			s->destroy();
			return true;
		}
	}

	return false;
}


