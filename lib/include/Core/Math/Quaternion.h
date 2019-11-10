#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix.h"
namespace fm
{
	namespace math
	{
		class Quaternion
		{
		public:
			Quaternion();
			
			Quaternion(const vec4& inVec4);
			operator vec4() const { return fm::math::vec4(x, y, z, w); }
			vec3 GetEulerAngles() const;
			static Quaternion FromEulerAngles(const vec3& inRotation);
			Quaternion Conjugate(const Quaternion& inQuaternion);
			mat  GetRotationMatrix() const;
		private:
			float w, x, y, z;
		};
	}
}
