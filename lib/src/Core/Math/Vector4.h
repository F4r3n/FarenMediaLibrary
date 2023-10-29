#pragma once
#include "Vector.h"
#include <nlohmann/json.hpp>

namespace fm {
	namespace math {
		typedef vec<float, 4> vec4;
		typedef vec<int, 4> vec4i;
		typedef vec<double, 4> vec4d;
		typedef vec<unsigned int, 4> vec4ui;

		template <typename T>
		void to_json(nlohmann::json& j, const vec<T, 4>& p) {
			j = nlohmann::json{ p.x, p.y, p.z, p.w };
		}
		template <typename T>
		void from_json(const nlohmann::json& j, vec<T, 4>& p) {
			p.x = j[0];
			p.y = j[1];
			p.z = j[2];
			p.w = j[3];
		}
	}

}

