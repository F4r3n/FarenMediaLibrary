#pragma once
#include <Entity.h>
#include "Core/Math/Vector2.h"
namespace fm 
{
	class GameObject;
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
		PickingSystem();
		fm::GameObject* PickGameObject(fm::GameObject* inCurrentCamera, fm::math::vec2 &inPos);
		
	private:
		fm::GameObject* _specialCamera;
	};
}