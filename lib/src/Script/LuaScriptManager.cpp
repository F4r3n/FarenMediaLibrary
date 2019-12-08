#include "Script/LuaScriptManager.h"
#include "Script/LuaScript.h"
#include <ECS.h>
#include <nlohmann/json.hpp>
#include "Core/FilePath.h"
using namespace fm;

namespace Keys
{
	const std::string script("script");
	const std::string kind("kind");
	const std::string path("path");
}


LuaScriptManager::LuaScriptManager()
{

}

LuaScriptManager::~LuaScriptManager()
{

}

void LuaScriptManager::init(Entity* e)
{
	sol::state *lua = (LuaManager::get().GetState());

	sol::table tempGoClass = (*lua)["GameObject"];
	_table = tempGoClass["create"](tempGoClass);


	for (auto &s : _scripts)
	{
		s->init(e);
		s->SetGoTable(_table);
	}
}

void LuaScriptManager::RemoveScript(const std::string &name)
{
	size_t index = 0;
	size_t indexFound = 0;
	bool found = false;
	for (auto &s : _scripts)
	{
		if (s->GetScriptName().compare(name) == 0)
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



void LuaScriptManager::update(Entity *e, float dt)
{
	for (auto &s : _scripts)
	{
		s->update(dt);
	}
}

void LuaScriptManager::Start(Entity* e)
{
	for (auto &s : _scripts)
	{
		s->start();
	}
}

void LuaScriptManager::Stop(Entity* e)
{
	for (auto &s : _scripts)
	{
		s->Stop(e);
	}
}

bool LuaScriptManager::Serialize(nlohmann::json &ioJson) const
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
bool LuaScriptManager::Read(Entity* e, const nlohmann::json &inJSON)
{
	nlohmann::json jarray = inJSON[Keys::script];
	for (const auto &it : jarray)
	{
		fm::FilePath path(it[Keys::path]);
		fm::Script::SCRIPT_TYPE type = it[Keys::kind];

		switch (type)
		{
		case fm::Script::SCRIPT_TYPE::LUA:
			_scripts.emplace_back(new fm::LuaScript(path, e));
			break;
		}
	}
	return true;
}

void LuaScriptManager::addScriptLua(Entity* e, const fm::FilePath &inPath)
{
	_scripts.emplace_back(new fm::LuaScript(inPath, e));
}

void LuaScriptManager::ReloadScript(Entity* e, const std::string &inName)
{
	for (auto &s : _scripts)
	{
		if (s->GetScriptName() == inName)
		{
			s->Reload(e);
			s->SetGoTable(_table);

			break;
		}
	}
}

void LuaScriptManager::CallEvent(fm::BaseEvent* inEvent)
{
	for (auto &s : _scripts)
	{
		s->CallEvent(inEvent, _table);
	}
}

