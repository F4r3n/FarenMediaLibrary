#pragma once
#include "Script.h"
#include <sol2/sol_forward.hpp>

class Entity;

namespace sol
{
	class state;
}

namespace fmc
{
	class CTransform;
}

class GameObjectLua
{
public:
	GameObjectLua(Entity* inEntity);
	fmc::CTransform* GetTransform();
private:
	Entity* _entity;
};


class LuaManager {
    
    public:
    LuaManager();
    ~LuaManager();
    inline static LuaManager& get() {
        return instance;
    }

	void openLibraries();
    void registerComponents();
	bool ReadFile(const std::string &inPath);

	sol::state* GetState() { return lua; }
private:
	void _OpenInternalLibs();


    static LuaManager instance;
    sol::state *lua;
	
};
