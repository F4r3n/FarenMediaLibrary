#pragma once
#include <Entity.h>
#include <SystemManager.h>
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

private:
    SystemManager systems;
};
}