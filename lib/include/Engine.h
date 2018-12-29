#pragma once


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
    
    
    void SetMainCamera(fm::GameObject *go);
    GameObject* GetMainCamera();
    void RunMainLoop(void *window);

    private:
        std::unique_ptr<SystemManager> _systems;
        fm::GameObject *_mainCamera;

        bool _hasStopped = false;

        size_t _numberFramesTimer = 0;
};
}
