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
    static std::shared_ptr<Entity> createEntity();
    void addSystem(std::shared_ptr<System> system)
    {
        systems.addSystem(system);
    }
    void update(float dt);

private:
    SystemManager systems;
};
}