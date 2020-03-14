#pragma once


#include "Script/LuaManager.h"
#include <sol2/sol.hpp>
#include <string>

class Entity;

namespace fm
{
	class BaseEvent;
	class FilePath;
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
		void CallEvent(fm::BaseEvent* inEvent, sol::table &inTable);

	    fm::Script::SCRIPT_TYPE GetType() const override
	    {
	        return fm::Script::SCRIPT_TYPE::LUA;
	    }
		virtual bool Reload(Entity*) override;
		void SetGoTable(sol::table &inTable);

	 private:
		bool			_hasAnErrorOccured;
		sol::table		_table;	
	};
}
