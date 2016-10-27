#pragma once
#include <ECS.h>
#include "Window.h"
#include "Speaker.h"
#include "Listener.h"

namespace fm
{
    
    enum SYSTEMS {RENDERER, SCRIPTER, PHYSIC, SOUND, LAST_SYSTEM};
class Engine
{
public:
    Engine();
    ~Engine();
    static Entity* createEntity();
   
    void update(float dt);
    void init();
    void start();
    void run(Window &window);
private:
    SystemManager systems;
    Speaker speaker;
    Listener listener;
};
}