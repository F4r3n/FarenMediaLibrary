#include "Components/CScriptManager.h"
#include "Script/Script.h"
using namespace fmc;
const std::string CScriptManager::name = "Script Manager";

CScriptManager::CScriptManager() {
    _name = "ScriptManager";
}

CScriptManager::~CScriptManager() {
}

void CScriptManager::init( Entity* e) {
    for(auto s : scripts) {
        s->init(e);
    }
}


void CScriptManager::update() {
    for(auto s : scripts) {
        if(!s->hasStarted) {
            s->start();
            s->hasStarted = true;
        }
        s->update();
    }
}

void CScriptManager::addScript(std::shared_ptr<fm::Script> file) {
    scripts.push_back(file);
}
