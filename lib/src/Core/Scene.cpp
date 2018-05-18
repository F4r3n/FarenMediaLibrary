#include "Core/Scene.h"
#include "Core/GameObject.h"
using namespace fm;

Scene::Scene(const std::string &name) {
    _name = name;
}

Scene::~Scene() {
}

void Scene::destroy() {
    for(auto e : _entities) {
        e->destroy();
    }
}

void Scene::AddGameObject(GameObject *e)
{
    _entities.push_back(e);
}

void Scene::Serialize(json &outJson)
{

    json scene;
    for(auto e : _entities)
    {
        json v;
        e->Serialize(v);
        scene[std::to_string(e->getID())] = v;
    }
    outJson[_name] = scene;
}
