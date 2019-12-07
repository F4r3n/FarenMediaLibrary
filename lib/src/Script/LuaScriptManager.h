#pragma once
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <sol2/sol.hpp>

class Entity;
namespace fm
{
	class LuaScript;
	class FilePath;
	class BaseEvent;
}


namespace fmc
{
	typedef std::vector<std::shared_ptr<fm::LuaScript> > LuaScripts;
}

namespace fm
{
	class LuaScriptManager
	{
	public:
		LuaScriptManager();
		~LuaScriptManager();

		void init(Entity* e);
		void update(Entity *e, float dt);
		void Start(Entity* e);
		void Stop(Entity* e);
		void CallEvent(fm::BaseEvent* inEvent);
		void addScriptLua(Entity* e, const fm::FilePath &inpath);
		void ReloadScript(Entity* e, const std::string &inName);
		void RemoveScript(const std::string &name);


		bool Serialize(nlohmann::json &ioJson) const;
		bool Read(Entity* e, const nlohmann::json &inJSON);

		fmc::LuaScripts GetLuaScripts() const { return _scripts; }
	private:
		sol::table _table;
		fmc::LuaScripts _scripts;
	};
}