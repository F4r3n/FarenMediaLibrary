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
    if(id < _entities.size())
        return _entities[id];
    return nullptr;
}


Scene::~Scene()
{
}

void Scene::destroy()
{
    for(auto e : _entities)
    {
        e->destroy();
    }
}

void Scene::AddGameObject(GameObject *e)
{

    _entities.push_back(e);
}

void Scene::Serialize(nlohmann::json &outJson)
{

    nlohmann::json entities;
    for(auto e : _entities)
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
