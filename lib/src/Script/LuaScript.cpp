#include "Script/LuaScript.h"
#include "Script/ScriptRegister.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include <sol2/sol.hpp>
#include "Core/FilePath.h"
#include "Core/Debug.h"
#include <Components/cevent.hpp>
#include <Components/CBody.h>
#include <ECS.h>
#include <Components/CScriptManager.h>
#include "LuaScriptManager.h"

#include <nlohmann/json.hpp>

using namespace fm;

namespace Keys
{
	const std::string startWith("startWith");

}


void fm::to_json(nlohmann::json& j, const LuaScript::ScriptArgument& p)
{
	std::string l;
	LuaScript::Tokens2Str(p.location, l);
	j["location"] = l;
	j["kind"] = (size_t)p.typeKind;
	if (p.typeKind == LuaScript::ScriptTypeValue::NUMERAL)
	{
		j["value"] = std::any_cast<float>(p.value);
	}
	else if (p.typeKind == LuaScript::ScriptTypeValue::STRING)
	{
		j["value"] = std::any_cast<std::string>(p.value);
	}
}

void fm::from_json(const nlohmann::json& j, LuaScript::ScriptArgument& p)
{
	p.location = LuaScript::CreateTokens(j["location"]);
	p.typeKind = j["kind"];
	if (p.typeKind == LuaScript::ScriptTypeValue::NUMERAL)
	{
		p.value = (float)j["value"];
	}
	else if (p.typeKind == LuaScript::ScriptTypeValue::STRING)
	{
		p.value = (std::string)j["value"];
	}
}



LuaScript::LuaScript(const fm::File &inFile, bool inParseInitValue)
{
	_file = inFile;
	_scriptName = _file.GetPath().GetName(true);
	_hasStarted = false;
	_isInit = false;
	Load(inParseInitValue);
}

bool LuaScript::Load(bool inParseInitValue)
{
	bool ok = false;
	try
	{
		if (LuaManager::get().ReadFile(_file.GetPathString()) && inParseInitValue)
		{
			sol::state* lua = (LuaManager::get().GetState());
			sol::table cclass = (*lua)[_scriptName];

			_RegisterInitVariable(cclass, "");
			ok = true;
		}
	}
	catch (std::exception& e)
	{
		std::string error = "Lua error " + _file.GetPathString();
		error += std::string(e.what());
		fm::Debug::get().LogError(error);
	}

	return ok;
}

void LuaScript::_RegisterInitVariable(const sol::table& inTable, const std::string& inKey)
{

	for (auto&& pair : inTable)
	{
		std::string key = pair.first.as<std::string>();
		if (!inKey.empty())
		{
			key = inKey + "." + key;
		}
		sol::type type = pair.second.get_type();

		ScriptArgument arg;
		arg.location = CreateTokens(key);
		bool valid = false;
		if (type == sol::type::number)
		{
			arg.typeKind = ScriptTypeValue::NUMERAL;
			arg.value = pair.second.as<float>();
			valid = true;
		}
		else if (type == sol::type::string)
		{
			arg.typeKind = ScriptTypeValue::STRING;
			arg.value = std::string(pair.second.as<char*>());
			valid = true;
		}
		else if (type == sol::type::table)
		{
			_RegisterInitVariable(pair.second.as<sol::table>(), key);
		}

		if (valid)
		{
			_valuesToInit.insert(std::pair<std::string, ScriptArgument>(key, arg));
		}
	}
}



LuaScript::~LuaScript()
{
}


void LuaScript::start()
{
	if (_hasAnErrorOccured || !_isInit) return;


	for (auto&& value : _valuesToInit)
	{
		SetValue(value.second);
	}
	

	try
	{
		sol::protected_function pf = _table["start"];
		if (pf.valid())
		{
			pf(_table);
		}
		else
		{
			fm::Debug::get().LogError("Cannot call start");
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

void LuaScript::Stop(const Entity& e)
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

bool LuaScript::Reload(bool inCreateInstance)
{
	bool resultScript = Load(true);

	if (resultScript && inCreateInstance)
	{
		bool ok = init();
		return ok && !_hasAnErrorOccured;
	}

	return resultScript;
}

bool LuaScript::Serialize(nlohmann::json &ioJson) const
{
	//Set values from inspector
	nlohmann::json j;
	for (auto&& v : _valuesToInit)
	{
		j[v.first] = v.second;
	}
	ioJson[Keys::startWith] = j;
	return true;
}

bool LuaScript::Read(const nlohmann::json &inJSON)
{
	try
	{
		nlohmann::json jarray = inJSON.at(Keys::startWith);
		for (nlohmann::json::const_iterator it = jarray.cbegin(); it != jarray.cend(); ++it)
		{
			_valuesToInit[it.key()] = it.value();
		}
	}
	catch (const std::exception&)
	{

	}
	

	return true;
}

void LuaScript::SetGoTable(sol::table &inTable)
{
	_table["Go"] = inTable;
}


bool LuaScript::init()
{
	sol::state *lua = (LuaManager::get().GetState());
	sol::table cclass = (*lua)[_scriptName];

	_table = lua->create_table();
	_table[sol::metatable_key] = cclass;
	cclass["__index"] = cclass;

	_isInit = true;
	_hasAnErrorOccured = false;
	_hasStarted = false;
    return _isInit && !_hasAnErrorOccured;
}


void LuaScript::CallEvent(fm::BaseEvent* inEvent, sol::table &inTable)
{
	if (inEvent->GetType() == EventKind::COLLISION)
	{
		fm::CollisionEvent* collisionEvent = dynamic_cast<fm::CollisionEvent*>(inEvent);
		Entity other = EntityManager::get().GetEntity(collisionEvent->GetID());
		if (other.Valid())
		{
			fmc::CScriptManager* mgr = other.get<fmc::CScriptManager>();
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

const std::map<std::string, LuaScript::ScriptArgument>& LuaScript::GetListValues() const
{
	return _valuesToInit;
}


LuaScript::Tokens LuaScript::CreateTokens(const std::string& inValueName)
{
	std::string currentStr;
	Tokens value;
	for (size_t i = 0; i < inValueName.size(); ++i)
	{
		if (inValueName[i] == '.')
		{
			value.push_back(currentStr);
			currentStr.clear();
		}
		else
		{
			currentStr += inValueName[i];
		}
	}
	value.push_back(currentStr);

	return value;
}


void LuaScript::EvaluateVariable(const ScriptArgument& inSpecialValue, std::any& out) const
{
	sol::table t = _table;
	for (size_t i = 0; i < inSpecialValue.location.size(); ++i)
	{
		if (!t.valid())
		{
			break;
		}

		if (i == inSpecialValue.location.size() - 1)
		{
			auto value = t[inSpecialValue.location[i]];
			if(inSpecialValue.typeKind == ScriptTypeValue::NUMERAL && value.get_type() == sol::type::number)
				out = value.get<float>();
			else if (inSpecialValue.typeKind == ScriptTypeValue::STRING && value.get_type() == sol::type::string)
				out = std::string(value.get<char*>());
		}
		else
		{
			t = t[inSpecialValue.location[i]];
		}
	}

}

bool LuaScript::SetValue(const ScriptArgument& inSpecialValue)
{
	sol::table t = _table;
	for (size_t i = 0; i < inSpecialValue.location.size(); ++i)
	{
		if (!t.valid())
		{
			return false;
		}

		if (i == inSpecialValue.location.size() - 1)
		{
			if (inSpecialValue.typeKind == ScriptTypeValue::NUMERAL)
				t[inSpecialValue.location[i]] = std::any_cast<float>(inSpecialValue.value);
			else if(inSpecialValue.typeKind == ScriptTypeValue::STRING)
				t[inSpecialValue.location[i]] = std::any_cast<std::string>(inSpecialValue.value);
			else if (inSpecialValue.typeKind == ScriptTypeValue::LUA)
				t[inSpecialValue.location[i]] = std::any_cast<sol::table>(inSpecialValue.value);

			return true;
		}
		else
		{
			t = t[inSpecialValue.location[i]];
		}
	}

	return false;
}



void LuaScript::Tokens2Str(const Tokens& inTokens, std::string& outText)
{
	for (size_t i = 0; i < inTokens.size(); ++i)
	{
		outText.append(inTokens[i]);
		if (i != inTokens.size() - 1)
		{
			outText.push_back('.');
		}
	}
}
