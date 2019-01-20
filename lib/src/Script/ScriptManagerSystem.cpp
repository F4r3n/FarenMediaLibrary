#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
using namespace fms;
ScriptManagerSystem::ScriptManagerSystem()
{
    LuaManager::get().openLibraries();
    CPPManager::get().LoadPlugin();
}


ScriptManagerSystem::~ScriptManagerSystem()
{
}

void ScriptManagerSystem::init(EntityManager& em, EventManager& event)
{
    fm::Debug::log("INIT scriptSystem");

    for(auto &&e : em.iterate<fmc::CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->init(e);
    }
}

void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager& event)
{
    for(auto &&e : em.iterate<fmc::CScriptManager>())
    {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->update(e, dt);
    }
}
