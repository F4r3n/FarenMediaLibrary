#include "Core/Scene.h"
#include "Core/GameObject.h"
#include <nlohmann/json.hpp>

using namespace fm;

Scene::Scene(const std::string &name)
{
    _name = name;
}

GameObject* Scene::GetGameObject(size_t id) const
{
    if(id < _gos.size() && id >= 0)
        return _gos[id];
    return nullptr;
}


Scene::~Scene()
{
}

void Scene::destroy()
{
    for(auto e : _gos)
    {
        e->destroy();
    }
	_gos.clear();
}

void Scene::AddGameObject(GameObject *e)
{
	_gos.push_back(e);
}

void Scene::Serialize(nlohmann::json &outJson)
{
    nlohmann::json entities;
    for(auto e : _gos)
    {
        nlohmann::json v;
        e->Serialize(v);
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
        GameObject *go = GameObjectHelper::create(this);
        go->Read(*it);
    }
    EntityManager::get().make();

    return true;
}

void Scene::SetStatusToGo(bool inStatus)
{
	for (auto& go : _gos)
	{
		go->SetStatus(false);
	}
}

void Scene::ResetStatusGo()
{
	for (auto& go : _gos)
	{
		go->ResetStatus();
	}
}



