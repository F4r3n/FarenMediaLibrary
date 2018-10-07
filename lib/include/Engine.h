#pragma once

#include "Music/Speaker.h"
#include "Music/Listener.h"
#include <memory>
namespace fm {
    class GameObject;
	class Window;
}
class SystemManager;

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
    void Run(fm::Window &window);
    
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
        std::unique_ptr<SystemManager> _systems;
        fm::GameObject *_mainCamera;
        Speaker _speaker;
        Listener _listener;
        bool _hasStopped = false;

        size_t _numberFramesTimer = 0;
};
}
