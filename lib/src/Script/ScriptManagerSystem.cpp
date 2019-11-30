#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
using namespace fms;
ScriptManagerSystem::ScriptManagerSystem()
{

}


ScriptManagerSystem::~ScriptManagerSystem()
{
}

void ScriptManagerSystem::init(EntityManager& em, EventManager&)
{
	LuaManager::get().openLibraries();
	CPPManager::get().LoadPlugin();


    fm::Debug::log("INIT scriptSystem");


}

void ScriptManagerSystem::Start()
{
	for (auto &&e : EntityManager::get().iterate<fmc::CScriptManager>())
	{
		fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
		scriptManager->init(e);
		scriptManager->Start(e);

	}
}

void ScriptManagerSystem::Stop()
{
	for (auto &&e : EntityManager::get().iterate<fmc::CScriptManager>())
	{
		fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
		scriptManager->init(e);
	}
}


void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager&)
{
    for(auto &&e : em.iterate<fmc::CScriptManager>())
    {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->update(e, dt);
    }
}
