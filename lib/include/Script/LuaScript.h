#pragma once

#include "Core/Math/Vector2.h"
#include "Input/InputManager.h"
#include <vector>

#include "Script/LuaManager.h"
#include <sol2/sol.hpp>
#include <memory>
#include <string>
#include "Core/FilePath.h"
class Entity;




namespace fm {
class LuaScript : public Script {
public:
    LuaScript(const fm::FilePath &inPath, Entity* inEntity);
    ~LuaScript();
    void update();
    void start();
    bool init(Entity* e);

    fm::Script::SCRIPT_TYPE GetType() const override
    {
        return fm::Script::SCRIPT_TYPE::LUA;
    }
	virtual bool Reload() override;
 private:
	sol::table _table;

	GameObjectLua *_go;
	std::string _name;
	fm::FilePath _path;
};
}
