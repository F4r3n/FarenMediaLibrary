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
    
    void stop();
    void resume();
    void reset();
private:
    SystemManager systems;
    Speaker speaker;
    Listener listener;
    
    bool hasStopped = false;
};
}