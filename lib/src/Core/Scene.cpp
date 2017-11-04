#include "Core/Scene.h"

using namespace fm;

Scene::Scene(const std::string &name) {
    _name = name;
}

Scene::~Scene() {
}

void Scene::addEntity(Entity *e) {
    _entities.push_back(e);
}
void Scene::serialize() {
    
    for(auto e : _entities) {
        
    }
}