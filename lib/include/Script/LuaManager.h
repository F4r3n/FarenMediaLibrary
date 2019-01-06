#pragma once
#include "Script.h"
#include <sol2/sol_forward.hpp>
#include <Entity.h>
template <typename T> T* get(Entity* e) {
    assert(e);
    return e->get<T>();
}

template <typename T> T* add(Entity* e) {
    assert(e);
    T* t = e->addComponent<T>();
    assert(t);
    return t;
}

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
