#include "Core/Scene.h"
#include "Core/GameObject.h"
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
    for(auto e : _entities)
    {
        e->destroy();
    }
}

void Scene::AddGameObject(GameObject *e)
{

    _entities.push_back(e);
}

void Scene::Serialize(json &outJson)
{

    json entities;
    for(auto e : _entities)
    {
        json v;
        e->Serialize(v);
        entities.push_back(v);
    }
    outJson["entities"] = entities;
    outJson["id"] = 0;
}

bool Scene::Read(const json &inJson)
{
    json entities = inJson["entities"];

    for (nlohmann::json::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
    {
        std::cout << *it << std::endl;
        GameObject *go = GameObjectHelper::create(this);
        go->Read(*it);
    }
    EntityManager::get().make();

    return true;
}
