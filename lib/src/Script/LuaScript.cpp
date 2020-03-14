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
#include "LuaScriptManager.h"
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
		}
	}
	catch(std::exception &e)
	{
		std::string error = "Lua error " + inPath.GetPath();
		error += std::string(e.what());
		fm::Debug::get().LogError(error);
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
		sol::protected_function pf = _table["start"];
		if (pf.valid())
		{
			pf(_table);
		}
		_hasStarted = true;

	}
	catch(std::exception &e)
	{
		_hasStarted = false;
		_hasAnErrorOccured = true;

		fm::Debug::get().LogError(e.what());
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
		sol::protected_function pf = _table["update"];
		if (pf.valid())
		{
			pf(_table, dt);
		}
	}
	catch (std::exception &e)
	{
		fm::Debug::get().LogError(e.what());
		_hasAnErrorOccured = true;

	}
}

bool LuaScript::Reload(Entity* inEntity)
{
	bool resultScript = false;
	try
	{
		resultScript = LuaManager::get().ReadFile(_path.GetPath());
	}
	catch (std::exception &e)
	{
		std::string error = "Lua error " + _path.GetPath();
		error += std::string(e.what());
		fm::Debug::get().LogError(error);
	}
	if (resultScript)
	{
		bool ok = init(nullptr);
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

void LuaScript::SetGoTable(sol::table &inTable)
{
	_table["Go"] = inTable;
}


bool LuaScript::init(Entity*)
{

	sol::state *lua = (LuaManager::get().GetState());
	sol::table cclass = (*lua)[_scriptName];
	sol::protected_function createF = cclass["create"];

	bool ok = false;

	if (createF.valid())
	{
		try
		{
			_table = createF(cclass);
			ok = true;
		}
		catch (std::exception &e)
		{
			fm::Debug::get().LogError(e.what());
		}
	}


	_isInit = ok;
	_hasAnErrorOccured = !ok;
	_hasStarted = false;
    return ok;
}


void LuaScript::CallEvent(fm::BaseEvent* inEvent, sol::table &inTable)
{
	if (inEvent->GetType() == EventKind::COLLISION)
	{
		fm::CollisionEvent* collisionEvent = dynamic_cast<fm::CollisionEvent*>(inEvent);
		Entity* other = EntityManager::get().getEntity(collisionEvent->GetID());
		if (other != nullptr)
		{
			fmc::CScriptManager* mgr = other->get<fmc::CScriptManager>();
			sol::table t = inTable;
			if (mgr != nullptr)
			{
				t = mgr->GetLuaScriptManager()->GetTable();
			}
			
			sol::protected_function pf = _table["Collision"];
			if (pf.valid())
			{
				pf(_table, t, collisionEvent->GetTouchPoint(), collisionEvent->GetNormalPoint());
			}
		}
		//_table["Collision"](_table, dt);
	}
}
