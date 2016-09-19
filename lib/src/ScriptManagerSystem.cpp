#include "ScriptManagerSystem.h"
#include "Input.h"
#include "Vector2.h"
#include "CTransform.h"
#include "CScriptManager.h"
#include <chrono>
using namespace fms;
using namespace fmc;
using namespace fm;
std::shared_ptr<Entity> currentEntity = nullptr;
template <typename T> std::shared_ptr<T> get() {
    //std::cout << currentEntity << std::endl;
    if(currentEntity)
    return currentEntity->get<T>();
    else return nullptr;
}

ScriptManagerSystem::ScriptManagerSystem() {
    addComponent<CScriptManager>();
    lua.open_libraries();

    registerComponent<Vector2f>("Vector", "x", &Vector2f::x, "y", &Vector2f::y);

    registerComponent<CTransform>("CTransform",
                                  "position",
                                  &CTransform::position,
                                  "scale",
                                  &CTransform::scale,
                                  "rotation",
                                  &CTransform::rotation);

    lua.set_function("keyIsPressed", &Input::keyIsPressed);
    lua.set_function("getMousePositionX", &Input::getMousePositionX);
    lua.set_function("getMousePositionY", &Input::getMousePositionY);

    lua.set_function("get_Transform", &get<CTransform>);
}

ScriptManagerSystem::~ScriptManagerSystem() {
}

void ScriptManagerSystem::init(std::shared_ptr<Entity> e) {
    std::shared_ptr<CScriptManager> scriptManager = e->get<CScriptManager>();
    currentEntity = e;
    scriptManager->init(lua);
}

void ScriptManagerSystem::update(float dt, std::shared_ptr<Entity> e) {
    std::shared_ptr<CScriptManager> scriptManager = e->get<CScriptManager>();
    //currentEntity = e;
             //auto start = std::chrono::system_clock::now();
            scriptManager->update(dt, lua);

       // auto end = std::chrono::system_clock::now();
       // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
       // std::cout << "Time " << elapsed.count() << '\n';
}
