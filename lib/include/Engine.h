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
   
    void Update(float dt);
    void Init();
    void Start();
    void Run(Window &window);
    
    void Stop();
    void Resume();
    void Reset();
    
    template <typename T>
    inline T* GetSystem() {
       return _systems.getSystem<T>();
    }
    
    void SetMainCamera(fm::GameObject *go);
    GameObject* GetMainCamera();
    void RunMainLoop(void *window);

    private:
        SystemManager _systems;
        fm::GameObject *_mainCamera;
        Speaker _speaker;
        Listener _listener;
        bool _hasStopped = false;

        size_t _numberFramesTimer = 0;
};
}
