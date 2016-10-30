#include "Components/CScriptManager.h"

using namespace fmc;
const std::string CScriptManager::name = "Script Manager";

CScriptManager::CScriptManager() {
}

CScriptManager::~CScriptManager() {
}

void CScriptManager::init(sol::state& lua, Entity* e) {
    for(auto s : scripts) {
        s->init(lua, e);
    }
}

void CScriptManager::update(float dt, sol::state& lua) {
    for(auto s : scripts) {
        if(!s->hasStarted) {
            s->start(lua);
            s->hasStarted = true;
        }
        s->update(lua, dt);
    }
}

void CScriptManager::addScript(std::shared_ptr<fm::Script> file) {
    scripts.push_back(file);
}