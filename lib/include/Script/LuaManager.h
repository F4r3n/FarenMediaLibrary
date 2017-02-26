#pragma once
#include "Script.h"
#include <single/sol/sol.hpp>
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


class LuaManager {
    
    public:
    LuaManager();
    ~LuaManager();
    inline static LuaManager& get() {
        return instance;
    }
    
    template <typename T>
    decltype(auto) operator[](T && t) {
        return lua[t];
    }
    
    sol::state& getState() {
        return lua;
    }
    
    template <typename T, typename... Args> void registerComponent(const std::string& name, Args&&... args) {
        lua.new_usertype<T>(name, args...);
    }
    
    void openLibraries() {
        lua.open_libraries();
    }
    
    void registerComponents();
    
private:
    static LuaManager instance;
    sol::state lua;
};