#include <nlohmann/json.hpp>

#include "Core/Scene.h"

#include "Core/SceneManager.h"
#include "Core/GameObject.h"
using namespace fm;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

namespace keys
{
	const std::string scenes("Scenes");
	const std::string path("path");
}


void SceneManager::SetCurrentScene(const std::string &name, bool isPrivate)
{
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
			if (s->GetName() == name)
			{
				_currentScene = s;
				break;
			}
		}
	}
}


void SceneManager::Serialize(nlohmann::json &outjson)
{
	nlohmann::json json;

    for(auto &scene : _scenes)
    {
		if(fm::File(scene.second->GetPath()).Exist())
			json.push_back(scene.second->GetPath().GetFileSystemPath());
    }
	outjson[keys::scenes] = json;
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
		if (s->GetName() == inName)
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
		if (s->GetName() == inName)
		{
			s->Read(injson);
			return true;
		}
	}
	return false;
}


bool SceneManager::Read(const nlohmann::json &injson)
{
	try
	{
		nlohmann::json listOfScenes = injson.at(keys::scenes);

		for (nlohmann::json::const_iterator it = listOfScenes.cbegin(); it != listOfScenes.cend(); ++it)
		{
			std::string pathScene = (*it);
			_currentScene = std::make_shared<fm::Scene>(fm::FilePath(pathScene));
			//nlohmann::json o = it.value();
			//_currentScene->Read(o);
			AddScene(_currentScene);
		}
	}
	catch(std::out_of_range& e)
	{
		return false;
	}
    return true;
}


void SceneManager::AddScene(std::shared_ptr<fm::Scene> inScene)
{
	_scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(inScene->GetName(), inScene));
}


std::shared_ptr<fm::Scene> SceneManager::AddNewScene(const fm::FilePath& inPath)
{
	std::shared_ptr<fm::Scene> scene = std::make_shared<fm::Scene>(inPath);
    _scenes.insert(std::pair<std::string, std::shared_ptr<Scene>>(scene->GetName(), scene));

	return scene;
}


std::shared_ptr<Scene> SceneManager::GetScene(const std::string &name)
{
	if (!name.empty())
	{
		if (_currentScene->GetName() == name)
		{
			return _currentScene;
		}
		else if (name.back() == '_')//Is a private scene
		{
			for (auto&& s : _privateScenes)
			{
				if (s->GetName() == name)
					return s;
			}
		}
		else if (_scenes.find(name) != _scenes.end())
		{
			return _scenes[name];
		}
	}
    return nullptr;
}


std::shared_ptr<fm::Scene> SceneManager::AddPrivateScene(const std::string &inName)
{
	for (auto& s : _privateScenes)
	{
		if (s->GetName() == inName)
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
		scene.second->Destroy();
	}
	_scenes.clear();

	if (clearPrivate)
	{
		for (auto &scene : _privateScenes)
		{
			scene->Destroy();
		}
		_privateScenes.clear();
	}
}


bool SceneManager::ClearScene(const std::string &inName, bool isPrivate, bool remove)
{
	bool isFound = false;
	if (!isPrivate)
	{
		std::map<std::string, std::shared_ptr<Scene>>::iterator it = _scenes.find(inName);
		if (it != _scenes.end())
		{
			it->second->Destroy();
			if (remove)
			{
				_scenes.erase(it);
			}
			isFound = true;
		}
	}
	else
	{
		std::vector<std::shared_ptr<fm::Scene>>::iterator it = _privateScenes.begin();
		for (it; it != _privateScenes.end(); it++)
		{
			if ((*it)->GetName() == inName)
			{
				(*it)->Destroy();
				isFound = true;
				break;
			}
		}
		if (isFound)
		{
			_privateScenes.erase(it);
		}
	}

	return isFound;
}


std::shared_ptr<Scene> SceneManager::RenameScene(std::shared_ptr<Scene> inCurrentScene, const fm::FilePath& inPath)
{
	nlohmann::json json;
	inCurrentScene->Serialize(json);

	ClearScene(inCurrentScene->GetName(), false/*private*/, true/*remove*/);

	auto scene = AddNewScene(inPath);
	scene->Read(json);

	return scene;
}

