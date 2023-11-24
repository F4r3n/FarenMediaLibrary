#pragma once
#include <memory>
#include <vector>
#include "component.h"
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>
namespace fm
{
	class Script;
	class FilePath;
	class BaseEvent;
	class LuaScriptManager;
	class LuaScript;
	class GameObject;
}
class GameObjectLua;

namespace fmc {
	typedef std::vector<std::shared_ptr<fm::LuaScript> > LuaScripts;
	typedef std::vector<std::shared_ptr<fm::Script> > Scripts;
	class CScriptManager
	{

	public:
		CScriptManager();
		~CScriptManager();
		void init(entt::handle e);
		void update(entt::handle e, float dt);
		void Start(entt::handle e);
		void Stop(entt::handle e);
		void CallEvent(entt::registry& registry, fm::BaseEvent* inEvent);
		void addScriptLua(const fm::FilePath& inpath);
		void ReloadScript(const std::string& inName);
		void RemoveScript(const std::string& name);

		uint16_t GetType() const { return kScriptManager; }

		bool Serialize(nlohmann::json& ioJson) const;
		bool Read(const nlohmann::json& inJSON);


		LuaScripts GetLuaScripts() const;
		fm::LuaScriptManager* GetLuaScriptManager() { return _luaScriptManager.get(); }
		GameObjectLua* GetGameObjectLua() const;

	private:
		std::unique_ptr<fm::LuaScriptManager> _luaScriptManager;
		bool _hasStarted = false;
		bool _hasBeenInitialized = false;
	private:
		std::string _name;
	};
}
