#include "Core/Scene.h"
#include "Core/GameObject.h"
#include <nlohmann/json.hpp>

using namespace fm;

Scene::Scene(const std::string &name)
{
    _name = name;
}


Scene::~Scene()
{
}

void Scene::destroy()
{
    for(auto &&e : _gos)
    {
        e.second->destroy();
		delete e.second;
    }
	_gos.clear();
}

void Scene::AddGameObject(GameObject *e)
{
	_gos[e->getID()] = e;
}

void Scene::Serialize(nlohmann::json &outJson)
{
    nlohmann::json entities;
    for(auto e : _gos)
    {
        nlohmann::json v;
        e.second->Serialize(v);
        entities.push_back(v);
    }
    outJson["entities"] = entities;
    outJson["id"] = 0;
}

bool Scene::Read(const nlohmann::json &inJson)
{
    nlohmann::json entities = inJson["entities"];

    for (nlohmann::json::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
    {
        GameObject *go = GameObjectHelper::create(nullptr, false);
		AddGameObject(go);
        go->Read(*it);
    }

    return true;
}

void Scene::SetStatusToGo(bool inStatus)
{
	for (auto& go : _gos)
	{
		go.second->SetStatus(false);
	}
}

void Scene::ResetStatusGo()
{
	for (auto& go : _gos)
	{
		go.second->ResetStatus();
	}
}

fm::GameObject* Scene::GetGameObjectByID(ecs::id inID)
{
	auto it = _gos.find(inID);
	if (it != _gos.end())
	{
		return it->second;
	}
	return nullptr;
}


void Scene::DeleteGameObjectByID(ecs::id inID)
{
	fm::GameObject* go = GetGameObjectByID(inID);
	if (go != nullptr)
	{
		go->destroy();
		delete go;
		_gos.erase(inID);
	}
}

