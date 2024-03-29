
#include "Core/Math/Quaternion.h"
#include "Core/Math/Functions.h"
#include <cfloat>
using namespace fm::math;

//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
vec3 Quaternion::GetEulerAngles() const
{
	return fm::math::vec3(degrees(_Pitch()), degrees(_Yaw()), degrees(_Roll()));
}

float Quaternion::_Roll() const
{
	return static_cast<float>(atan2(static_cast<float>(2) * (x * y + w * z), w * w + x * x - y * y - z * z));
}

float Quaternion::_Pitch() const
{
	float const _y = static_cast<float>(2) * (y * z + w * x);
	float const _x = w * w - x * x - y * y + z * z;

	if (_x < FLT_EPSILON && _x >= 0 && _y < FLT_EPSILON && _y > 0) //avoid atan2(0,0) - handle singularity - Matiis
		return static_cast<float>(static_cast<float>(2) * atan2(x, w));

	return static_cast<float>(atan2(_y, _x));
}


float Quaternion::_Yaw() const
{
	return asin(clamp(static_cast<float>(-2) * (x * z - w * y), static_cast<float>(-1), static_cast<float>(1)));
}

Quaternion Quaternion::FromEulerAngles(const vec3& inRotation)
{
	fm::math::vec3d angle;
	angle.x = inRotation.x;
	angle.y = inRotation.y;
	angle.z = inRotation.z;

	angle.x = fm::math::radians(angle.x*0.5);
	angle.y = fm::math::radians(angle.y*0.5);
	angle.z = fm::math::radians(angle.z*0.5);

	// Abbreviations for the various angular functions
	const double cz = cos(angle.z);
	const double sz = sin(angle.z);
	const double cy = cos(angle.y);
	const double sy = sin(angle.y);
	const double cx = cos(angle.x);
	const double sx = sin(angle.x);

	Quaternion q;
	q.w = float(cx * cy * cz + sx * sy * sz);
	q.x = float(sx * cy * cz - cx * sy * sz);
	q.y = float(cx * sy * cz + sx * cy * sz);
	q.z = float(cx * cy * sz - sx * sy * cz);

	return q;

}

mat  Quaternion::GetRotationMatrix() const
{
	mat m;

	m[0][0] = 1 - 2 * (y*y + z * z);
	m[0][1] = 2 * (x*y + z * w);
	m[0][2] = 2 * (x*z - y * w);
	m[0][3] = 0;

	m[1][0] = 2 * (x*y - z * w);
	m[1][1] = 1 - 2 * (x*x + z * z);
	m[1][2] = 2 * (y*z + x * w);
	m[1][3] = 0;

	m[2][0] = 2 * (x*z + y * w);
	m[2][1] = 2 * (y*z - x * w);
	m[2][2] = 1 - 2 * (x*x + y * y);
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
}


Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) 
{}

Quaternion Quaternion::Conjugate(const Quaternion& inQuaternion)
{
	return Quaternion(inQuaternion.w, -inQuaternion.x, -inQuaternion.y, -inQuaternion.z);
}

Quaternion Quaternion::Rotate(const Quaternion& q)
{
	Quaternion p(*this);
	this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
	this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
	return *this;
}

fm::math::Quaternion Quaternion::operator *(const fm::math::Quaternion& q) const
{
	const float _w = w * q.w - x * q.x - y * q.y - z * q.z;
	const float _x = w * q.x + x * q.w + y * q.z - z * q.y;
	const float _y = w * q.y + y * q.w + z * q.x - x * q.z;
	const float _z = w * q.z + z * q.w + x * q.y - y * q.x;

	return Quaternion(_w, _x, _y, _z);
}

fm::math::Quaternion Quaternion::Conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

float Quaternion::LengthSquared() const
{
	return w * w + x * x + y * y + z * z;
}

fm::math::Quaternion Quaternion::operator *(float v) const
{
	return Quaternion(w * v, x * v, y * v, z * v);
}


fm::math::Quaternion Quaternion::Inverse() const
{
	return Conjugate() * (1 / LengthSquared());
}





