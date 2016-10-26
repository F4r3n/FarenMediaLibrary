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
#include "CSource.h"
#include "Body2D.h"

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

    registerComponent<Vector2f>("Vector2f",sol::constructors<sol::types<float, float>>(),
 "x", &Vector2f::x, "y", &Vector2f::y);
    registerComponent<Vector2d>("Vector2d", "x", &Vector2d::x, "y", &Vector2d::y);
    registerComponent<Vertex>("Vertex", "position", &Vertex::position, "uv", &Vertex::uv);

    registerComponent<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
    registerComponent<CTransform>("CTransform",
                                  "position",
                                  &CTransform::position,
                                  "scale",
                                  &CTransform::scale,
                                  "rotation",
                                  &CTransform::rotation,
                                  "layer",
                                  &CTransform::layer);
    registerComponent<CMaterial>("CMaterial", "color", &CMaterial::color);
    registerComponent<CSource>("CSource", "play", &CSource::play, "status", &CSource::getStatus);
    registerComponent<CMesh>("CMesh", "setShape", &CMesh::setShape, "create", &CMesh::create);
    registerComponent<Body2D>("Body2D", 
    "applyForceCenter", &Body2D::applyForceCenter2, 
    "friction", &Body2D::friction);

    registerComponent<Entity>("Entity",
                              "ID",
                              sol::readonly(&Entity::ID),
                              "getTransform",
                              &Entity::get<CTransform>,
                              "getMaterial",
                              &Entity::get<CMaterial>,
                              "getSource",
                              &Entity::get<CSource>,
                              "getBody",
                              &Entity::get<Body2D>
                              );
    registerComponent<Collision>("Collision", "ID", sol::readonly(&Collision::id));
    registerComponent<ColliderInfo>("ColliderInfo", "ID", sol::readonly(&ColliderInfo::idOther));

    registerComponent<Input>("Input",
                             "getMouseButton",
                             &Input::getMouseButton,
                             "keyIsPressed",
                             &Input::keyIsPressed,
                             "getMousePosition",
                             &Input::getMousePositionVector,
                             "getMouseButton",
                             &Input::getMouseButton);

    lua.set_function("add_Transform", &add<CTransform>);
    lua.set_function("add_Mesh", &add<CMesh>);
    lua.set_function("add_Material", &add<CMaterial>);

    lua.set_function("createEntity", &createEntity);
}

void ScriptManagerSystem::receive(const Collision& collision) {
    Entity* e = EntityManager::get().getEntity(collision.owner);

    if(e && e->has<fmc::CScriptManager>() && e->ID != collision.owner) {
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        scriptManager->event("CollisionEvent", lua, collision);
    }
}

void ScriptManagerSystem::receive(const Collider& collider) {

    processCollision(collider.idA, collider.idB, collider.event);
    processCollision(collider.idB, collider.idA, collider.event);
}

void ScriptManagerSystem::processCollision(size_t idA, size_t idB, EVENT_COLLISION event) {
    Entity* e = EntityManager::get().getEntity(idB);

    if(e && e->has<fmc::CScriptManager>()) {
        ColliderInfo info;
        info.idOther = idA;
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        if(event == EVENT_COLLISION::BEGIN) {
            scriptManager->event("CollisionEventEnter", lua, info);

        } else if(event == EVENT_COLLISION::END) {
            scriptManager->event("CollisionEventExit", lua, info);
        }
    }
}

ScriptManagerSystem::~ScriptManagerSystem() {
}

void ScriptManagerSystem::init(EntityManager& em, EventManager& event) {
    event.subscribe<Collision>(*this);
    event.subscribe<Collider>(*this);
    for(auto e : em.iterate<CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        scriptManager->init(lua, e);
    }
}

void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager& event) {
    for(auto e : em.iterate<CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
        scriptManager->update(dt, lua);
    }
}
