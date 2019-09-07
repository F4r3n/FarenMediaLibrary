#pragma once
#include "Core/Math/Vector3.h"

namespace fm
{
	struct Transform
	{
		fm::math::Vector3f position = { 0, 0, 0 };
		fm::math::Vector3f scale = { 1, 1, 1 };
		fm::math::Vector3f rotation = { 0,0,0 };
	};
}