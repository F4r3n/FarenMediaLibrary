#include "Core/Math/Quaternion.h"
#include "Core/Math/Functions.h"
using namespace fm::math;

//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
vec3 Quaternion::GetEulerAngles() const
{
	vec3 angle;
	// roll (x-axis rotation)
	const double sinr_cosp = 2 * (_q.w * _q.x + _q.y * _q.z);
	const double cosr_cosp = 1 - 2 * (_q.x * _q.x + _q.y * _q.y);
	angle.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	const double sinp = 2 * (_q.w * _q.y - _q.z * _q.x);
	if (std::abs(sinp) >= 1)
		angle.y = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angle.y = std::asin(sinp);

	// yaw (z-axis rotation)
	const double siny_cosp = 2 * (_q.w * _q.z + _q.x * _q.y);
	const double cosy_cosp = 1 - 2 * (_q.y * _q.y + _q.z * _q.z);
	angle.z = std::atan2(siny_cosp, cosy_cosp);

	return angle;
}

void Quaternion::FromEulerAngles(const vec3& inRotation)
{
	// Abbreviations for the various angular functions
	const double cy = cos(inRotation.x * 0.5);
	const double sy = sin(inRotation.x * 0.5);
	const double cp = cos(inRotation.y * 0.5);
	const double sp = sin(inRotation.y * 0.5);
	const double cr = cos(inRotation.z * 0.5);
	const double sr = sin(inRotation.z * 0.5);

	_q.w = cy * cp * cr + sy * sp * sr;
	_q.x = cy * cp * sr - sy * sp * cr;
	_q.y = sy * cp * sr + cy * sp * cr;
	_q.z = sy * cp * cr - cy * sp * sr;

}

mat  Quaternion::GetRotationMatrix() const
{
	mat m;

	m[0][0] = _q.x*_q.x + _q.y*_q.y - _q.z*_q.z - _q.w*_q.w;
	m[0][1] = 2 * (_q.y*_q.z - _q.x*_q.w);
	m[0][2] = 2 * (_q.x*_q.z + _q.y*_q.w);
	m[0][3] = 0;

	m[1][0] = 2 * (_q.y*_q.z + _q.x*_q.w);
	m[1][1] = _q.x*_q.x - _q.y*_q.y + _q.z*_q.z - _q.w*_q.w;
	m[1][2] = 2 * (_q.z*_q.w - _q.x*_q.y);
	m[1][3] = 0;

	m[2][0] = 2 * (_q.y*_q.w - _q.x*_q.z);
	m[2][1] = 2 * (_q.x*_q.y + _q.z*_q.w);
	m[2][2] = _q.x*_q.x - _q.y*_q.y - _q.z*_q.z + _q.w*_q.w;
	m[2][3] = 0;

	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;

	return m;
}

Quaternion::Quaternion(const vec4& inVec4)
{
	_q = inVec4;
}

Quaternion::Quaternion()
{
	_q.x = 0;
	_q.y = 0;
	_q.z = 0;
	_q.w = 1;
}



Quaternion Quaternion::Conjugate(const Quaternion& inQuaternion)
{
	return Quaternion(_q * inQuaternion._q);
}


