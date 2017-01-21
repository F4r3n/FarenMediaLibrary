#pragma once
#include <single/sol/sol.hpp>
#include "Vector2.h"
#include "InputManager.h"
#include <vector>
#include "Entity.h"
namespace fm
{
class Script
{
public:
    Script(const std::string& script, const std::string& nameVariable ="");
    Script();
    ~Script();
    bool attachScript(const std::string& script);
    void update(sol::state& lua);
    void start(sol::state& lua);
    bool init(sol::state& lua, Entity *e);
  
    void setGameObjects(const std::string &name, Entity* var) {
        objects[name] = var;
    }
    //void test() {}
    template <typename T>
    void event(std::string name, sol::state& lua, const T &t) {
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
    //Transform transform;
    sol::state tempLua;
    bool isDrawable = false;
    std::unordered_map<std::string, Entity*> objects;
};
}