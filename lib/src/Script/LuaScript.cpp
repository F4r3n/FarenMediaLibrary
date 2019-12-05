#include "Script/LuaScript.h"
#include "Script/ScriptRegister.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include <sol2/sol.hpp>
#include "Core/FilePath.h"
#include "Core/Debug.h"
#include <Components/cevent.hpp>
#include <Components/CBody3D.h>
#include <ECS.h>
#include <Components/CScriptManager.h>
using namespace fm;



LuaScript::LuaScript(const fm::FilePath &inPath, Entity* inEntity)
{
	_path = inPath;
	_scriptName = inPath.GetName(true);
	_hasStarted = false;
	_isInit = false;
	try
	{
		if (LuaManager::get().ReadFile(inPath.GetPath()))
		{
			_go = new GameObjectLua(inEntity);
		}
	}
	catch(std::exception &e)
	{
		fm::Debug::get().logError(e.what());
	}
}



LuaScript::~LuaScript()
{
}


void LuaScript::start()
{
	if (_hasAnErrorOccured || !_isInit) return;
	try
	{
		_table["start"](_table);
		_hasStarted = true;

	}
	catch(std::exception &e)
	{
		_hasStarted = false;
		_hasAnErrorOccured = true;

		fm::Debug::get().logError(e.what());
	}
}

void LuaScript::Stop(Entity* e)
{
	_hasStarted = false;
	_isInit = false;
	_hasAnErrorOccured = false;
	_table = nullptr;
}


void LuaScript::update(float dt)
{
	if (_hasAnErrorOccured || !_hasStarted || !_isInit) return;

	try
	{
		_table["update"](_table, dt);
	}
	catch (std::exception &e)
	{
		fm::Debug::get().logError(e.what());
		_hasAnErrorOccured = true;

	}
}

bool LuaScript::Reload()
{
	bool resultScript = false;
	try
	{
		resultScript = LuaManager::get().ReadFile(_path.GetPath());
	}
	catch (std::exception &e)
	{
		fm::Debug::get().logError(e.what());
	}
	if (resultScript)
	{
		bool ok = init(nullptr);
		if (ok)
		{
			start();
		}
		return ok && !_hasAnErrorOccured;
	}

	return resultScript;
}

bool LuaScript::Serialize(nlohmann::json &ioJson) const
{
	//Set values from inspector
	return false;
}

bool LuaScript::Read(const nlohmann::json &inJSON)
{
	return false;
}

bool LuaScript::init(Entity*)
{
	if (_go == nullptr) return false;

	sol::state *lua = (LuaManager::get().GetState());
	sol::table cclass = (*lua)[_scriptName];
	_table = cclass["create"](cclass);

	sol::table tempGoClass = (*lua)["GameObject"];
	_table["Go"] = tempGoClass["create"](tempGoClass);
	_table["Go"]["_internal"] = _go;
	_isInit = true;
	_hasAnErrorOccured = false;
	_hasStarted = false;
    return true;
}


void LuaScript::CallEvent(fm::BaseEvent* inEvent)
{
	if (inEvent->GetType() == EventKind::COLLISION)
	{
		fm::CollisionEvent* collisionEvent = dynamic_cast<fm::CollisionEvent*>(inEvent);
		Entity* other = EntityManager::get().getEntity(collisionEvent->GetID());
		if (other != nullptr)
		{
			std::shared_ptr<fm::LuaScript> luaScript = std::dynamic_pointer_cast<fm::LuaScript>(other->get<fmc::CScriptManager>()->GetScripts().front());
			_table["Collision"](_table, luaScript->_table["Go"]);
		}
		//_table["Collision"](_table, dt);
	}
}
