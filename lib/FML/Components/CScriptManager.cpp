#include "Components/CScriptManager.h"
#include "Script/Script.h"
#include "Core/FilePath.h"
#include "Script/LuaScript.h"
#include <nlohmann/json.hpp>
#include "Script/LuaScriptManager.h"
#include "Core/GameObject.h"
using namespace fmc;


CScriptManager::CScriptManager()
{
    _name = "ScriptManager";
	_luaScriptManager = std::unique_ptr<fm::LuaScriptManager>(new fm::LuaScriptManager());
}

CScriptManager::~CScriptManager()
{

}


void CScriptManager::init(entt::handle e)
{
	_luaScriptManager->init(e);
}

void CScriptManager::RemoveScript(const std::string &name)
{
	_luaScriptManager->RemoveScript(name);
}



void CScriptManager::update(entt::handle e, float dt)
{
	_luaScriptManager->update(e, dt);
}

void CScriptManager::Start(entt::handle e)
{
	_luaScriptManager->Start(e);
}

void CScriptManager::Stop(entt::handle e)
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

void CScriptManager::CallEvent(entt::registry& registry, fm::BaseEvent* inEvent)
{
	_luaScriptManager->CallEvent(registry, inEvent);
}

LuaScripts CScriptManager::GetLuaScripts() const
{
	return _luaScriptManager->GetLuaScripts();
}


GameObjectLua* CScriptManager::GetGameObjectLua() const
{
	return _luaScriptManager->GetGameObjectLua();
}



