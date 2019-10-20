#pragma once
#include <Entity.h>
#include "Core/Math/Vector2.h"
#include <functional>
namespace fm 
{
	class GameObject;
	class Material;
	class Scene;
}

class EntityManager;
namespace fmc
{
	class CCamera;
}

namespace fms
{
	class PickingSystem 
	{
	public:
		PickingSystem(std::shared_ptr<fm::Scene> inEditorScene);
		void PickGameObject(ecs::id inCameraID, const fm::math::vec2 &inPos);
		void SetCallback(std::function<void(fm::GameObject*)> &&inCallback) { _callback = inCallback; }
		int _toto;
	private:
		std::function<void(fm::GameObject*)> _callback;
		fm::GameObject* _specialCamera;
		fmc::CCamera* _camera;
		std::unique_ptr<fm::Material> _material;
		std::shared_ptr<fm::Scene> _editorScene;
		std::shared_ptr<fm::Scene> _currentScene;

	};
}