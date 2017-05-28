#pragma once
#include <single/sol/sol.hpp>
#include "Core/Math/Vector2.h"
#include "Input/InputManager.h"
#include <vector>
#include "Entity.h"
#include "Script/LuaManager.h"
#include "Components/CCamera.h"


namespace fm {
class LuaScript : public Script {
public:
    LuaScript(const std::string& script, const std::string& nameVariable = "");
    LuaScript();
    ~LuaScript();
    bool attachScript(const std::string& script);
    void update();
    void start();
    bool init(Entity* e);

    void setGameObjects(const std::string& name, Entity* var) {
        objects[name] = var;
    }
    void event(std::string name, const CameraInfo& cam) {
        if(LuaManager::get()[nameVariable][name]) {
           LuaManager::get()[nameVariable][name](LuaManager::get()[nameVariable], cam.camera);
        }
    }

    void event(std::string name, const ColliderInfo& t) {
        if(LuaManager::get()[nameVariable][name]) {
            LuaManager::get()[nameVariable][name](LuaManager::get()[nameVariable], t);
        }
    }

    void event(std::string name, const Collider& t) {
        if(LuaManager::get()[nameVariable][name]) {
            LuaManager::get()[nameVariable][name](LuaManager::get()[nameVariable], t);
        }
    }

    std::string getName() const;
    void setName(const std::string& name);

    template <typename T> T getComponent(const std::string name) {
        return (T)LuaManager::get()[nameVariable][name];
    }
    bool hasStarted = false;
    void reload();
    private:

    std::string scriptName;
    std::string nameVariable;
    std::string nameFile;
    bool isDrawable = false;
    std::unordered_map<std::string, Entity*> objects;
};
}