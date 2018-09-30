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

class LuaManager {
    
    public:
    LuaManager();
    ~LuaManager();
    inline static LuaManager& get() {
        return instance;
    }
    
    //template <typename T>
    //decltype(auto) operator[](T && t) {
    //    return lua[t];
    //}
    
	sol::state* getState();
    //template <typename T, typename... Args> void registerComponent(const std::string& name, Args&&... args) {
    //    lua.new_usertype<T>(name, args...);
    //}
    
	void openLibraries();
    
    void registerComponents();
    
private:
    static LuaManager instance;
    sol::state *lua;
};
