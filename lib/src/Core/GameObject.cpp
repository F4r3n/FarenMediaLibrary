#include "Core/GameObject.h"
#include "Core/Scene.h"
using namespace fm;
GameObject* GameObject::create() {
    if(_entity != nullptr) return this;
    _entity = EntityManager::get().createEntity();
    Scene* s = SceneManager::get().getCurrentScene();
    s->addEntity(this);
    return this;
}
GameObject::GameObject() {
    name = "GameObject";
}
GameObject::~GameObject() {}
