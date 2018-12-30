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
    
    
    void RunMainLoop(void *window);

    private:
        std::unique_ptr<SystemManager> _systems;

        bool _hasStopped = false;

        size_t _numberFramesTimer = 0;
};
}
