#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
#include "Components/cevent.hpp"
#include "Engine.h"
using namespace fms;
ScriptManagerSystem::ScriptManagerSystem()
{

}


ScriptManagerSystem::~ScriptManagerSystem()
{
}

void ScriptManagerSystem::init(EventManager&)
{
	LuaManager::get().openLibraries();
	CPPManager::get().LoadPlugin();


    fm::Debug::log("INIT scriptSystem");


}

void ScriptManagerSystem::Start(entt::registry& registry)
{
	auto view = registry.view<fmc::CScriptManager>();
	for (auto e : view)
	{
		fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
		scriptManager.init(entt::handle(registry, e));
	}

	for (auto e : view)
	{
		fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
		scriptManager.Start(entt::handle(registry, e));
	}
}

void ScriptManagerSystem::Stop(entt::registry& registry)
{
	
}


void ScriptManagerSystem::update(float dt, entt::registry& registry, EventManager&)
{
	for (auto e : registry.view<fmc::CScriptManager, fmc::CEvent>())
	{
		fmc::CEvent &event = registry.get<fmc::CEvent>(e);
		fmc::Events events = event.GetEvents();
		fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);

		for (const auto &a : events)
		{
			for (const auto &b : a)
			{
				scriptManager.CallEvent(registry, b);
			}
		}
		event.Clear();
	}


    for(auto e : registry.view<fmc::CScriptManager>())
    {
        fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
        scriptManager.update(entt::handle(registry,e), dt);
    }
}
