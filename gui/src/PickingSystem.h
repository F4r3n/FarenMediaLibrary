#pragma once

#include "Core/Math/Vector2.h"
#include <functional>
#include <FML/Core/GameObjectType.hpp>
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
		void PickGameObject(const std::string& inSceneName, fm::GameObjectID_t inCameraID, const fm::math::vec2 &inPos, std::function<void(fm::GameObjectID_t)> inCallback = {});
		void SetCallback(std::function<void(fm::GameObjectID_t)> &&inCallback) { _callback = inCallback; }
	private:
		std::function<void(fm::GameObjectID_t)> _callback;
		std::weak_ptr<fm::GameObject> _specialCamera;
		std::shared_ptr<fm::Material> _material;
		std::weak_ptr<fm::Scene> _editorScene;

	};
}