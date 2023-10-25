#pragma once
#include <cmath>
#include <ostream>
#include "Vector.h"
#include <nlohmann/json_fwd.hpp>


namespace fm {
	namespace math {
		typedef vec<float, 3> Vector3f;
		typedef vec<float, 3> vec3;

		typedef vec<int, 3> Vector3i;
		typedef vec<double, 3> Vector3d;
		typedef vec<double, 3> vec3d;

		typedef vec<unsigned int, 3> Vector3ui;

		void to_json(nlohmann::json& j, const vec3& p);
		void from_json(const nlohmann::json& j, vec3& p);

	}
}

