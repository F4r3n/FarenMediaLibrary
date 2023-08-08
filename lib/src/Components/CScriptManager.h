#pragma once
#include <memory>
#include <vector>
#include "component.h"
#include <nlohmann/json_fwd.hpp>
#include "Entity.h"
 
namespace fm
{
class Script;
class FilePath;
class BaseEvent;
class LuaScriptManager;
class LuaScript;
}
class Entity;
class GameObjectLua;

namespace fmc {
	typedef std::vector<std::shared_ptr<fm::LuaScript> > LuaScripts;
	typedef std::vector<std::shared_ptr<fm::Script> > Scripts;
class CScriptManager : public FMComponent<CScriptManager> {

    public:
        CScriptManager();
        ~CScriptManager();
        void init(const Entity::Id& e);
        void update(const Entity& e, float dt);
		void Start(const Entity& e);
		void Stop(const Entity& e);
		void CallEvent(fm::BaseEvent* inEvent);
		void addScriptLua(const fm::FilePath &inpath);
		void ReloadScript(const std::string &inName);
        void RemoveScript(const std::string &name);

        virtual uint16_t GetType() const {return kScriptManager;}

		bool Serialize(nlohmann::json &ioJson) const override;
		bool Read(const nlohmann::json &inJSON) override;


		LuaScripts GetLuaScripts() const;
		fm::LuaScriptManager* GetLuaScriptManager() { return _luaScriptManager.get(); }
		GameObjectLua* GetGameObjectLua() const;

private:
	std::unique_ptr<fm::LuaScriptManager> _luaScriptManager;
};
}
