#pragma once
#include <single/sol/sol.hpp>
#include "Vector2.h"
#include "InputManager.h"
#include <vector>
#include "Entity.h"
#include "LuaManager.h"
namespace fm
{
class LuaScript : public Script
{
public:
    LuaScript(const std::string& script, const std::string& nameVariable ="");
    LuaScript();
    ~LuaScript();
    bool attachScript(const std::string& script);
    void update();
    void start();
    bool init(Entity *e);
  
    void setGameObjects(const std::string &name, Entity* var) {
        objects[name] = var;
    }
    //void test() {}
    
    void event(std::string name, sol::state& lua, const ColliderInfo &t) {
        if(lua[nameVariable][name]) {
            lua[nameVariable][name](lua[nameVariable], t);
        
        }
    }
    
    void event(std::string name, sol::state& lua, const Collider &t) {
    if(lua[nameVariable][name]) {
        lua[nameVariable][name](lua[nameVariable], t);
        
        }
    }
    
    
    std::string getName() const;
    void setName(const std::string& name);

    template <typename T> T getComponent(sol::state& lua, const std::string name)
    {
        return (T)lua[nameVariable][name];
    }
    bool hasStarted = false;
private:
    std::string scriptName;
    std::string nameVariable;
    std::string nameFile;
    bool isDrawable = false;
    std::unordered_map<std::string, Entity*> objects;
};
}