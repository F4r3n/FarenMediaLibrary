#pragma once


#include "Script/LuaManager.h"
#include <sol2/sol.hpp>
#include <string>
#include "Core/FilePath.h"

class Entity;

namespace fm
{
	class BaseEvent;
}

namespace fm
{
	class LuaScript : public Script
	{


	public:
	    LuaScript(const fm::FilePath &inPath, Entity* inEntity);
	    ~LuaScript();
	    virtual void update(float dt) override;
	    virtual void start() override;
	    virtual bool init(Entity* e) override;
		virtual void Stop(Entity* e) override;
		virtual bool Serialize(nlohmann::json &ioJson) const override;
		virtual bool Read(const nlohmann::json &inJSON) override;
		virtual void CallEvent(fm::BaseEvent* inEvent);

	    fm::Script::SCRIPT_TYPE GetType() const override
	    {
	        return fm::Script::SCRIPT_TYPE::LUA;
	    }
		virtual bool Reload() override;


	 private:
		bool			_hasAnErrorOccured;
		sol::table		_table;
	
		GameObjectLua*	_go;
	};
}
