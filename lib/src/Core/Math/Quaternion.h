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
			Quaternion(const Quaternion &q);
			Quaternion(float w, float x, float y, float z);
			Quaternion(const vec4& inVec4);
			operator vec4() const { return fm::math::vec4(x, y, z, w); }
			vec3 GetEulerAngles() const;
			static Quaternion FromEulerAngles(const vec3& inRotation);
			static Quaternion Conjugate(const Quaternion& inQuaternion);
			Quaternion Rotate(const Quaternion& q);
			mat  GetRotationMatrix() const;

		private:
			float _Roll() const;
			float _Pitch() const;
			float _Yaw() const;

			float w, x, y, z;
		};
	}
}
