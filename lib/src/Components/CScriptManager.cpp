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


void CScriptManager::Destroy()
{
    EntityManager::get().removeComponent<CScriptManager>(BaseComponent::_IDEntity);
}


void CScriptManager::init(Entity* e)
{
	_luaScriptManager->init(e);
}

void CScriptManager::RemoveScript(const std::string &name)
{
	_luaScriptManager->RemoveScript(name);
}



void CScriptManager::update(Entity *e, float dt)
{
	_luaScriptManager->update(e, dt);
}

void CScriptManager::Start(Entity* e)
{
	_luaScriptManager->Start(e);
}

void CScriptManager::Stop(Entity* e)
{
	_luaScriptManager->Stop(e);
}

bool CScriptManager::Serialize(nlohmann::json &ioJson) const
{
	return _luaScriptManager->Serialize(ioJson);
}
bool CScriptManager::Read(const nlohmann::json &inJSON)
{
	Entity* e = EntityManager::get().getEntity(BaseComponent::_IDEntity);
	return _luaScriptManager->Read(e, inJSON);
}

void CScriptManager::addScriptLua(const fm::FilePath &inPath)
{
	Entity* e = EntityManager::get().getEntity(BaseComponent::_IDEntity);
	_luaScriptManager->addScriptLua(e, inPath);
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



