#pragma once
#include <ECS.h>
#include "Window.h"
#include "Music/Speaker.h"
#include "Music/Listener.h"

namespace fm {
    class GameObject;
}

namespace fm
{
  
class Engine
{
public:
    Engine();
    ~Engine();
   
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
    
    void setMainCamera(fm::GameObject *go);
    GameObject* getMainCamera();
    void loop(void *window);

    private:
    SystemManager systems;
    fm::GameObject *mainCamera;
    Speaker speaker;
    Listener listener;
    bool hasStopped = false;
    
    size_t numberFramesTimer = 0;
};
}
