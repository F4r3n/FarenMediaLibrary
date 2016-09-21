#include "ScriptManagerSystem.h"
#include "Input.h"
#include "Vector2.h"
#include "CTransform.h"
#include "CScriptManager.h"
#include "Color.h"
#include "CMaterial.h"
#include <chrono>
using namespace fms;
using namespace fmc;
using namespace fm;

template <typename T> T* get(Entity *e) {
    //std::cout << currentEntity << std::endl;
    if(e)
    return e->get<T>();
    else return nullptr;
}

ScriptManagerSystem::ScriptManagerSystem() {
    addComponent<CScriptManager>();
    lua.open_libraries();

    registerComponent<Vector2f>("Vector", "x", &Vector2f::x, "y", &Vector2f::y);
    registerComponent<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
    registerComponent<CTransform>("CTransform",
                                  "position",
                                  &CTransform::position,
                                  "scale",
                                  &CTransform::scale,
                                  "rotation",
                                  &CTransform::rotation);
    registerComponent<CMaterial>("CMaterial", "color", &CMaterial::color);
    //registerComponent<Entity>("Entity","ID", sol::readonly(&Entity::ID), "getTransform", &Entity::get<CTransform>);
    lua.set_function("keyIsPressed", &Input::keyIsPressed);
    lua.set_function("getMousePositionX", &Input::getMousePositionX);
    lua.set_function("getMousePositionY", &Input::getMousePositionY);

    lua.set_function("get_Transform", &get<CTransform>);
    lua.set_function("get_Material", &get<CMaterial>);
}

ScriptManagerSystem::~ScriptManagerSystem() {
}

void ScriptManagerSystem::init(Entity* e) {
    // std::cout << "Script" << std::endl;
    fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
    scriptManager->init(lua, e);
    
}

void ScriptManagerSystem::update(float dt, Entity* e) {
    fmc::CScriptManager* scriptManager = e->get<CScriptManager>();
    scriptManager->update(dt, lua);

}
