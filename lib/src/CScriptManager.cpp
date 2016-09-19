#include "CScriptManager.h"

using namespace fmc;

CScriptManager::CScriptManager() {
}

CScriptManager::~CScriptManager() {
}

void CScriptManager::init(sol::state& lua) {
    for(auto s : scripts) {
        s->init(lua);
    }
}

void CScriptManager::update(float dt, sol::state &lua) {
       for(auto s : scripts) {
        s->update(lua, dt);
    }
}


void CScriptManager::addScript(std::shared_ptr<fm::Script> file) {
    scripts.push_back(file);
}