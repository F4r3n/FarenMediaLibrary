#pragma once


#include <memory>
#include "SystemManager.h"
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
    
    
	SYSTEM_MANAGER_MODE GetStatus() const;

    private:
        std::unique_ptr<SystemManager>	_systems;
};
}
