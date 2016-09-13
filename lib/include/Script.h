#pragma once
#include <single/sol/sol.hpp>
#include "Vector2.h"
#include "InputManager.h"
#include <vector>
#include "Component.h"
namespace fm
{

class Script
{
public:
    Script(const std::string& script);
    Script();
    ~Script();
    bool attachScript(const std::string& script);
    void update(sol::state& lua, float dt);
    void start(sol::state& lua);
    bool init(sol::state& lua);
    //Transform getTransform() const;
    std::string getName() const;
    void setName(const std::string& name);

    template <typename T> T getComponent(sol::state& lua, const std::string name)
    {
        return (T)lua[nameVariable][name];
    }

private:
    std::string scriptName;
    std::string nameVariable;
    std::string nameFile;
    //Transform transform;

    bool isDrawable = false;
};
}