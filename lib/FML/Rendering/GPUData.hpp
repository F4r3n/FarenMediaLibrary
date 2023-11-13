#pragma once
#include "Core/Math/Matrix44.h"
#include "Core/Math/Vector4.h"
namespace fms
{
	struct GPUObjectData
	{
		fm::math::mat modelMatrix;
	};

	struct GPUSceneData {
		fm::math::vec4 fogColor; // w is for exponent
		fm::math::vec4 fogDistances; //x for min, y for max, zw unused.
		fm::math::vec4 ambientColor;
		fm::math::vec4 sunlightDirection; //w for sun power
		fm::math::vec4 sunlightColor;
	};
}