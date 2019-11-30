#include "Components/CScriptManager.h"
#include "Script/Script.h"
#include <EntityManager.h>
#include "Core/FilePath.h"
#include "Script/LuaScript.h"
#include <nlohmann/json.hpp>
using namespace fmc;

namespace Keys
{
	const std::string script("script");
	const std::string kind("kind");
	const std::string path("path");

}

CScriptManager::CScriptManager()
{
    _name = "ScriptManager";
}

CScriptManager::~CScriptManager()
{

}

void CScriptManager::init( Entity* e)
{
    for(auto &s : _scripts)
	{
        s->init(e);
    }
}

void CScriptManager::Destroy()
{
    EntityManager::get().removeComponent<CScriptManager>(BaseComponent::_IDEntity);
}

void CScriptManager::RemoveScript(const std::string &name)
{
    size_t index = 0;
    size_t indexFound = 0;
    bool found = false;
    for(auto &s : _scripts)
    {
        if(s->GetScriptName().compare(name) == 0)
        {
            indexFound = index;
            found = true;
            break;
        }
        index++;
    }
	if (found)
	{
		_scripts.erase(_scripts.begin() + indexFound);
	}
}



void CScriptManager::update(Entity *e, float dt)
{
    for(auto &s : _scripts)
    {
        s->update(dt);
    }
}

void CScriptManager::Start(Entity* e)
{
	for (auto &s : _scripts)
	{
		s->start();
	}
}

void CScriptManager::Stop(Entity* e)
{
	for (auto &s : _scripts)
	{
		s->Stop(e);
	}
}

bool CScriptManager::Serialize(nlohmann::json &ioJson) const
{
	nlohmann::json jarray;
	for (const auto &script : _scripts)
	{
		nlohmann::json j;
		script->Serialize(j);
		j[Keys::kind] = script->GetType();
		j[Keys::path] = script->GetFilePath().GetPath();
		jarray.push_back(j);
	}
	ioJson[Keys::script] = jarray;
	return true;
}
bool CScriptManager::Read(const nlohmann::json &inJSON)
{
	nlohmann::json jarray = inJSON[Keys::script];
	for (const auto &it : jarray)
	{
		fm::FilePath path(it[Keys::path]);
		fm::Script::SCRIPT_TYPE type = it[Keys::kind];

		switch (type)
		{
		case fm::Script::SCRIPT_TYPE::LUA:
			Entity* e = EntityManager::get().getEntity(BaseComponent::_IDEntity);
			_scripts.emplace_back(new fm::LuaScript(path, e));
			break;
		}
	}
	return true;
}

void CScriptManager::addScriptLua(const fm::FilePath &inPath)
{
	Entity* e = EntityManager::get().getEntity(BaseComponent::_IDEntity);
	_scripts.emplace_back(new fm::LuaScript(inPath, e));
}

void CScriptManager::ReloadScript(const std::string &inName)
{
	for (auto &s : _scripts)
	{
		if (s->GetScriptName() == inName)
		{
			s->Reload();
		}
	}
}

