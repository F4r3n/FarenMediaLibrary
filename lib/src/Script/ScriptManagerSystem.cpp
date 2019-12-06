#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
#include "Components/cevent.hpp"
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
	
}


void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager&)
{
	for (auto &&e : em.iterate<fmc::CScriptManager, fmc::CEvent>())
	{
		fmc::CEvent *event = e->get<fmc::CEvent>();
		fmc::Events &&events = event->GetEvents();
		fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();

		for (const auto &a : events)
		{
			for (const auto &b : a)
			{
				scriptManager->CallEvent(b);
			}
		}
	}


    for(auto &&e : em.iterate<fmc::CScriptManager>())
    {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->update(e, dt);
    }
}
