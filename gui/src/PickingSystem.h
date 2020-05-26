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
		void PickGameObject(const std::string& inSceneName, size_t inCameraID, const fm::math::vec2 &inPos);
		void SetCallback(std::function<void(ecs::id inID)> &&inCallback) { _callback = inCallback; }
		int _toto;
	private:
		std::function<void(ecs::id inID)> _callback;
		std::shared_ptr<fm::GameObject> _specialCamera;
		fmc::CCamera* _camera;
		std::unique_ptr<fm::Material> _material;
		std::shared_ptr<fm::Scene> _editorScene;

	};
}