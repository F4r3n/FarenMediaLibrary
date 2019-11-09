#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix.h"
namespace fm
{
	struct Transform //Assume all is in worldPos
	{
		fm::math::mat worldTransform;

		fm::math::vec3 GetPosition() const { return fm::math::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]); }
	private:

	};
}