#include "Script/LuaScript.h"
#include "Script/ScriptRegister.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include <sol2/sol.hpp>
#include "Core/FilePath.h"
using namespace fm;



LuaScript::LuaScript(const fm::FilePath &inPath, Entity* inEntity)
{
	_path = inPath;
	_name = inPath.GetName(true);
	if (LuaManager::get().ReadFile(inPath.GetPath()))
	{
		_go = new GameObjectLua(inEntity);
	}
	//_class = LuaManager::get().GetState();
}



LuaScript::~LuaScript()
{
}


void LuaScript::start()
{
	_table["start"](_table);
    hasStarted = true;
}

void LuaScript::update()
{
	_table["update"](_table);
}

bool LuaScript::Reload()
{
	if (LuaManager::get().ReadFile(_path.GetPath()))
	{
		return init(nullptr);
	}

	return false;
}


bool LuaScript::init(Entity* e)
{
	sol::state *lua = (LuaManager::get().GetState());
	sol::table cclass = (*lua)[_name];
	_table = cclass["create"](cclass);
	
	sol::table tempGoClass = (*lua)["GameObject"];
	_table["Go"] = tempGoClass["create"](tempGoClass);
	_table["Go"]["_internal"] = _go;
	isInit = true;
    return true;
}
