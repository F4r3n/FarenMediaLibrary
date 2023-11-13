#include "Components/CScriptManager.h"
#include "Script/Script.h"
#include <EntityManager.h>
#include "Core/FilePath.h"
#include "Script/LuaScript.h"
#include <nlohmann/json.hpp>
#include "Script/LuaScriptManager.h"
using namespace fmc;


CScriptManager::CScriptManager()
{
    _name = "ScriptManager";
	_luaScriptManager = std::unique_ptr<fm::LuaScriptManager>(new fm::LuaScriptManager());
}

CScriptManager::~CScriptManager()
{

}


void CScriptManager::init(const Entity::Id& e)
{
	_luaScriptManager->init(EntityManager::get().GetEntity(e));
}

void CScriptManager::RemoveScript(const std::string &name)
{
	_luaScriptManager->RemoveScript(name);
}



void CScriptManager::update(const Entity& e, float dt)
{
	_luaScriptManager->update(e, dt);
}

void CScriptManager::Start(const Entity& e)
{
	_luaScriptManager->Start(e);
}

void CScriptManager::Stop(const Entity& e)
{
	_luaScriptManager->Stop(e);
}

bool CScriptManager::Serialize(nlohmann::json &ioJson) const
{
	return _luaScriptManager->Serialize(ioJson);
}
bool CScriptManager::Read(const nlohmann::json &inJSON)
{
	return _luaScriptManager->Read(inJSON);
}

void CScriptManager::addScriptLua(const fm::FilePath &inPath)
{
	_luaScriptManager->addScriptLua(inPath);
}

void CScriptManager::ReloadScript(const std::string &inName)
{
	_luaScriptManager->ReloadScript(inName);
}

void CScriptManager::CallEvent(fm::BaseEvent* inEvent)
{
	_luaScriptManager->CallEvent(inEvent);
}

LuaScripts CScriptManager::GetLuaScripts() const
{
	return _luaScriptManager->GetLuaScripts();
}


GameObjectLua* CScriptManager::GetGameObjectLua() const
{
	return _luaScriptManager->GetGameObjectLua();
}



