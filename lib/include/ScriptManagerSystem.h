#pragma once
#include <System.h>
#include <Entity.h>
#include <Event.h>
#include <single/sol/sol.hpp>
#include "EventComponents.h"
namespace fms {
     
class ScriptManagerSystem : public System, public Receiver<ScriptManagerSystem> {
public:
    ScriptManagerSystem();
    ~ScriptManagerSystem();
    void pre_update(EntityManager& em) {}
    void update(float dt, EntityManager& em, EventManager &event);
    void init(EntityManager& em, EventManager &event);
    void over() {
    }
    void receive(const Collision &collision);
   

private:
    template <typename T, typename... Args> void registerComponent(const std::string& name, Args&&... args) {
        lua.new_usertype<T>(name, args...);
    }
   
    sol::state lua;
};
}