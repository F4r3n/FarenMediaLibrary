#pragma once
#include <ECS.h>
#include "Window.h"
#include "Music/Speaker.h"
#include "Music/Listener.h"

namespace fm
{
  
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
    
    template <typename T>
    inline T* getSystem() {
       return systems.getSystem<T>(); 
    }
    
    void setMainCamera();
    Entity* getMainCamera();
    void loop(void *window);

    private:
    SystemManager systems;
    Speaker speaker;
    Listener listener;
    Entity* camera;
    bool hasStopped = false;
    
    size_t numberFramesTimer = 0;
};
}