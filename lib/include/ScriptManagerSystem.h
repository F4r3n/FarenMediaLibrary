#pragma once
#include <System.h>
#include <Entity.h>
#include <single/sol/sol.hpp>
namespace fms {
class ScriptManagerSystem : public System {
public:
    ScriptManagerSystem();
    ~ScriptManagerSystem();

    void update(float dt, Entity* e);
    void init(Entity* e);
    void over() {
    }
   

private:
    template <typename T, typename... Args> void registerComponent(const std::string& name, Args&&... args) {
        lua.new_usertype<T>(name, args...);
    }
    sol::state lua;
    
};
}