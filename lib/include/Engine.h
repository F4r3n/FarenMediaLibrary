#pragma once
#include <ECS.h>
#include "Window.h"
#include "Speaker.h"
#include "Listener.h"
namespace fm
{
class Engine
{
public:
    Engine();
    ~Engine();
    static Entity* createEntity();
    void addSystem(std::shared_ptr<System> system)
    {
        systems.addSystem(system);
    }
    void update(float dt);
    void init();
    void run(Window &window);
private:
    SystemManager systems;
    Speaker speaker;
    Listener listener;
};
}