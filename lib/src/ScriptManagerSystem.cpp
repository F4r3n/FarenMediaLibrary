#include "ScriptManagerSystem.h"
#include "Input.h"
#include "Vector2.h"
#include "CTransform.h"
#include "CScriptManager.h"
#include "Color.h"
#include "CMaterial.h"
#include <chrono>
#include "EntityManager.h"
#include <cassert>
#include "CMesh.h"
using namespace fms;
using namespace fmc;
using namespace fm;

template <typename T> T* get(Entity* e) {
    assert(e);
    return e->get<T>();
}

template <typename T> T* add(Entity* e) {
    assert(e);
    T* t = e->addComponent<T>();
    assert(t);
    return t;
}

Entity* createEntity() {
    return EntityManager::get().createEntity();
}

ScriptManagerSystem::ScriptManagerSystem() {
    addComponent<CScriptManager>();
    lua.open_libraries();

    registerComponent<Vector2f>("Vector2f", "x", &Vector2f::x, "y", &Vector2f::y);
    registerComponent<Vertex>("Vertex", "position", &Vertex::position, "uv", &Vertex::uv);

    registerComponent<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
    registerComponent<CTransform>("CTransform",
                                  "position",
                                  &CTransform::position,
                                  "scale",
                                  &CTransform::scale,
                                  "rotation",
                                  &CTransform::rotation, "layer", &CTransform::layer);
    registerComponent<CMaterial>("CMaterial", "color", &CMaterial::color);
    registerComponent<CMesh>("CMesh", "setShape", &CMesh::setShape, "create", &CMesh::create);

    registerComponent<Entity>("Entity",
                              "ID",
                              sol::readonly(&Entity::ID),
                              "getTransform",
                              &Entity::get<CTransform>,
                              "getMaterial",
                              &Entity::get<CMaterial>

                              );
    registerComponent<Collision>("Collision", "ID", sol::readonly(&Collision::id));

    lua.set_function("keyIsPressed", &Input::keyIsPressed);
    lua.set_function("getMousePositionX", &Input::getMousePositionX);
    lua.set_function("getMousePositionY", &Input::getMousePositionY);
    lua.set_function("add_Transform", &add<CTransform>);
    lua.set_function("add_Mesh", &add<CMesh>);
    lua.set_function("add_Material", &add<CMaterial>);

    lua.set_function("createEntity", &createEntity);
    
    
}

 void ScriptManagerSystem::receive(const Collision &collision) {
     Entity *e = EntityManager::get().getEntity(e->ID);
     
     if( e && e->has<fmc::CScriptManager>() && e->ID == collision.owner) {
         fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
         scriptManager->event("CollisionEvent", lua, collision);
     }
 }

ScriptManagerSystem::~ScriptManagerSystem() {
}

void ScriptManagerSystem::init(EntityManager& em, EventManager &event) {
    event.subscribe<Collision>(*this);
    for(auto e : em.iterate<CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        scriptManager->init(lua, e);
    }
}

void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager &event) {
    for(auto e : em.iterate<CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        scriptManager->update(dt, lua);
    }
}
