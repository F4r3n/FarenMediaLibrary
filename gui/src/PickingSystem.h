#pragma once
#include <Entity.h>
#include "Core/Math/Vector2.h"
#include <functional>
#include "Entity.h"
namespace fm 
{
	class GameObject;
	class Material;
	class Scene;
}

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
		void PickGameObject(const std::string& inSceneName, size_t inCameraID, const fm::math::vec2 &inPos, std::function<void(Entity::Id)> inCallback = {});
		void SetCallback(std::function<void(Entity::Id)> &&inCallback) { _callback = inCallback; }
	private:
		std::function<void(Entity::Id)> _callback;
		std::weak_ptr<fm::GameObject> _specialCamera;
		fmc::CCamera* _camera;
		std::shared_ptr<fm::Material> _material;
		std::weak_ptr<fm::Scene> _editorScene;

	};
}