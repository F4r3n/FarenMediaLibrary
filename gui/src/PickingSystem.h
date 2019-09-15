#pragma once
#include <Entity.h>
#include "Core/Math/Vector2.h"
#include <functional>
namespace fm 
{
	class GameObject;
	class Material;
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
		PickingSystem(std::function<void(fm::GameObject*)> &&inCallback);
		fm::GameObject* PickGameObject(fm::GameObject* inCurrentCamera, fm::math::vec2 &inPos);
		 
	private:
		std::function<void(fm::GameObject*)> _callback;
		fm::GameObject* _specialCamera;
		fmc::CCamera* _camera;
		std::unique_ptr<fm::Material> _material;
	};
}