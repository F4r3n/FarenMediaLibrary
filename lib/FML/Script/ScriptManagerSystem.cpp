#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
#include "Core/Debug.h"
#include "Components/cevent.hpp"
#include "Engine.h"
#include "Components/CIdentity.h"
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
	_running = true;
}

void ScriptManagerSystem::Stop(entt::registry& registry)
{
	_running = false;
}


void ScriptManagerSystem::update(float dt, entt::registry& registry, EventManager&)
{
	if (_running)
	{
		auto view = registry.view<fmc::CScriptManager, fmc::CIdentity>();
		for (auto e : view)
		{
			const auto identity = registry.get<fmc::CIdentity>(e);
			if (!identity.IsActive())
				continue;

			fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
			scriptManager.init(entt::handle(registry, e));
		}

		for (auto e : view)
		{
			const auto identity = registry.get<fmc::CIdentity>(e);
			if (!identity.IsActive())
				continue;

			fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
			scriptManager.Start(entt::handle(registry, e));
		}
	}



	for (auto e : registry.view<fmc::CScriptManager, fmc::CEvent, fmc::CIdentity>())
	{
		fmc::CEvent &event = registry.get<fmc::CEvent>(e);
		fmc::Events events = event.GetEvents();
		fmc::CScriptManager& scriptManager = registry.get<fmc::CScriptManager>(e);
		const auto identity = registry.get<fmc::CIdentity>(e);
		if (!identity.IsActive())
			continue;

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
