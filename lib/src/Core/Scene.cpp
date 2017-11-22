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

void Scene::addEntity(GameObject *e) {
    _entities.push_back(e);
}
void Scene::serialize() {

    for(auto e : _entities) {

    }
}
