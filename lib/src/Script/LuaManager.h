#pragma once
#include "Script.h"
#include <sol2/forward.hpp>

class Entity;

namespace sol
{
	class state;
}

namespace fmc
{
	class CTransform;
	class CBody;
}

class GameObjectLua
{
public:
	GameObjectLua(const Entity& inEntity);
	fmc::CTransform* GetTransform();
	fmc::CBody* GetBody();
	const char* GetName();
private:
	Entity _entity;
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
