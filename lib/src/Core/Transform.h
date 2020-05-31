#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"
namespace fm
{
	struct Transform //Assume all is in worldPos
	{
		fm::math::mat worldTransform;
		fm::math::Quaternion worldRotation;
		fm::math::vec3 position;
		fm::math::vec3 scale;
		fm::math::Quaternion rotation;
		
	private:

	};
}