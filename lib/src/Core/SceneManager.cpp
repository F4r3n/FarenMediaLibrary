#include <nlohmann/json.hpp>

#include "Core/Scene.h"

#include "Core/SceneManager.h"
#include "Core/GameObject.h"
using namespace fm;
SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::setCurrentScene(const std::string &name, bool isPrivate) {

	if (!isPrivate)
	{
		std::map<std::string, std::shared_ptr<Scene>>::iterator it = _scenes.find(name);
		if (_scenes.find(name) != _scenes.end())
		{
			_currentScene = it->second;
		}
	}
	else
	{
		for (auto &s : _privateScenes)
		{
			if (s->getName() == name)
			{
				_currentScene = s;
				break;
			}
		}
	}


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

void SceneManager::SerializeCurrentScene(nlohmann::json &outjson)
{
	_currentScene->Serialize(outjson);
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
		_currentScene = std::make_shared<fm::Scene>(it.key());
        nlohmann::json o = it.value();
		_currentScene->Read(o);

        AddScene(_currentScene);
    }
    return true;
}

void SceneManager::AddScene(std::shared_ptr<fm::Scene> inScene)
{
	_scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(inScene->getName(), inScene));
}


void SceneManager::AddNewScene(const std::string &inName)
{
    _scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(inName, std::make_shared<Scene>(inName)));
}

std::shared_ptr<Scene> SceneManager::getScene(const std::string &name)
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

	return _privateScenes.emplace_back(std::make_shared<fm::Scene>(inName));
}

void SceneManager::ClearAll(bool clearPrivate)
{

	for (auto &scene : _scenes)
	{
		scene.second->destroy();
	}
	_scenes.clear();

	if (clearPrivate)
	{
		for (auto &scene : _privateScenes)
		{
			scene->destroy();
		}
		_privateScenes.clear();
	}
}



bool SceneManager::ClearScene(const std::string &inName, bool isPrivate)
{
	if (!isPrivate)
	{
		std::map<std::string, std::shared_ptr<Scene>>::iterator it = _scenes.find(inName);
		if (it != _scenes.end())
		{
			it->second->destroy();
			return true;
		}
	}
	else
	{
		for (auto& s : _privateScenes)
		{
			if (s->getName() == inName)
			{
				s->destroy();
				return true;
			}
		}
	}


	return false;
}


