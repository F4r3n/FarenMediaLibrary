#pragma once
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <sol2/sol.hpp>
#include <entt/entt.hpp>
class Entity;
namespace fm
{
	class LuaScript;
	class FilePath;
	class BaseEvent;
	class GameObject;
}
class GameObjectLua;

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

		void init(entt::handle e);
		void update(entt::handle e, float dt);
		void Start(entt::handle e);
		void Stop(entt::handle e);
		void CallEvent(entt::registry& registry, fm::BaseEvent* inEvent);
		void addScriptLua(const fm::FilePath &inpath);
		void ReloadScript(const std::string &inName);
		void RemoveScript(const std::string &name);


		bool Serialize(nlohmann::json &ioJson) const;
		bool Read(const nlohmann::json &inJSON);

		fmc::LuaScripts GetLuaScripts() const { return _scripts; }
		sol::table& GetTable();
		GameObjectLua* GetGameObjectLua() { return _go.get(); }
	private:
		sol::table _table;
		fmc::LuaScripts _scripts;
		std::unique_ptr<GameObjectLua>	_go;

	};
}