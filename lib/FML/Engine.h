#pragma once


#include <memory>
#include "Rendering/GraphicsAPI.h"
#include "Core/System/SystemManager.hpp"
namespace fm
{
    class GameObject;
	class Window;
	class Scene;
}

namespace fmc
{
	class CCamera;
	class CTransform;
}
class SystemManager;
namespace fm
{
class Engine
{
public:
    Engine();
    ~Engine();
   
    void Update(float dt, std::shared_ptr<fm::Scene> inScene);
	
    void Init(RENDERING_MODE inAPI, std::array<std::shared_ptr<fm::Window>, (int)GRAPHIC_API::LAST> window);
    void Start(std::shared_ptr<fm::Scene> inScene);
    
    void Stop(std::shared_ptr<fm::Scene> inScene);
    void Resume();
    void Reset();
	bool IsRunning() const { return _running; }

	void DrawScene(std::shared_ptr<fm::Scene> inScene, fmc::CCamera& inCamera, fmc::CTransform& inTransform);
    private:
		std::unordered_map<GRAPHIC_API, BaseSystem*> _renderings;
		bool _running = false;
        MapOfSystems _systems;
};
}
