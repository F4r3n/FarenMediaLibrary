#include "Core/Math/Quaternion.h"
#include "Core/Math/Functions.h"
using namespace fm::math;

//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
vec3 Quaternion::GetEulerAngles() const
{
	fm::math::vec3 angles;
	float test = x * y + z * w;
	if (test > 0.499)
	{
		angles.y = degrees(2 * std::atan2f(x, w));
		angles.z = 90;
		angles.x = 0;
	}
	else if (test < -0.499)
	{
		angles.y = -degrees(2 * std::atan2f(x, w));
		angles.z = -90;
		angles.x = 0;
	}
	else
	{
		angles.y = degrees(std::atan2f(2 * y*w - 2 * x*z, 1 - 2 * y*y - 2 * z*z));
		angles.z = degrees(std::asin(2 * x*y + 2 * z*w));
		angles.x = degrees(std::atan2f(2 * x*w - 2 * y*z, 1 - 2 * x*x + z * z));
	}

	return angles;
}

Quaternion Quaternion::FromEulerAngles(const vec3& inRotation)
{
	fm::math::vec3 angle = inRotation;
	//if (angle.x > 90) angle.x -= 180;
	//if (angle.y > 90) angle.y -= 180;
	//if (angle.z > 90) angle.z -= 180;

	angle.x = fm::math::radians(angle.x*0.5);
	angle.y = fm::math::radians(angle.y*0.5);
	angle.z = fm::math::radians(angle.z*0.5);

	// Abbreviations for the various angular functions
	double cy = cos(angle.z);
	double sy = sin(angle.z);
	double cp = cos(angle.y);
	double sp = sin(angle.y);
	double cr = cos(angle.x);
	double sr = sin(angle.x);

	Quaternion q;
	q.w = cy * cp * cr - sy * sp * sr;
	q.x = cy * cp * sr + sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;

	return q;

}

mat  Quaternion::GetRotationMatrix() const
{
	mat m;

	m[0][0] = 1 - 2*(y*y - z*z);
	m[0][1] = 2 * (x*y - z*w);
	m[0][2] = 2 * (x*z + y*w);
	m[0][3] = 0;

	m[1][0] = 2 * (x*y + z*w);
	m[1][1] = 1 - 2*(x*x - z*z);
	m[1][2] = 2 * (y*z - x*w);
	m[1][3] = 0;

	m[2][0] = 2 * (x*z - y*w);
	m[2][1] = 2 * (y*z + x*w);
	m[2][2] = 1 - 2*(x*x - y*y);
	m[2][3] = 0;

	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;

	return m;
}

Quaternion::Quaternion(const vec4& inVec4)
{
	w = inVec4.w;
	x = inVec4.x;
	y = inVec4.y;
	z = inVec4.z;

}

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}



Quaternion Quaternion::Conjugate(const Quaternion& inQuaternion)
{
	fm::math::vec4 v(x, y, z, w);
	fm::math::vec4 r = v * (vec4)inQuaternion;
	return r;
}


