#pragma once


#include <memory>
#include "SystemManager.h"
#include "Entity.h"
#include "Rendering/GraphicsAPI.h"
namespace fm {
    class GameObject;
	class Window;
}
class SystemManager;
namespace fm
{
	bool IsEntityActive(EntityManager& em, const Entity::Id& id);
class Engine
{
public:
    Engine();
    ~Engine();
   
    void Update(float dt);
    void Init(GRAPHIC_API inAPI, std::shared_ptr<fm::Window> window);
    void Start();
    void Run(std::shared_ptr<fm::Window> window);
    
    void Stop();
    void Resume();
    void Reset();
    
    
	SYSTEM_MANAGER_MODE GetStatus() const;

    private:
        std::unique_ptr<SystemManager>	_systems;
};
}
