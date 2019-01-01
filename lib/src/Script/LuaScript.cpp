#include "Script/LuaScript.h"
#include "Script/ScriptRegister.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include <sol2/sol.hpp>
#include "Core/FilePath.h"
using namespace fm;



LuaScript::LuaScript(const fm::FilePath &inPath, Entity* inEntity)
{
	//TODO check class
	_name = inPath.GetName(true);
	bool s = LuaManager::get().ReadFile(inPath.GetPath());
	//_class = LuaManager::get().GetState();
	_go = new GameObjectLua(inEntity);
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


bool LuaScript::init(Entity* e)
{
	sol::state *lua = (LuaManager::get().GetState());
	sol::table cclass = (*lua)[_name];
	_table = cclass["create"](cclass);

	_table["Go"] = (*lua)["GameObject"];
	_table["Go"]["_internal"] = _go;
	
    return true;
}
