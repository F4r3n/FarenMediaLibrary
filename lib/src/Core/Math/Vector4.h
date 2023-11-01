#pragma once
#include "Vector.h"
#include <nlohmann/json_fwd.hpp>

namespace fm {
	namespace math {
		typedef vec<float, 4> vec4;
		typedef vec<int, 4> vec4i;
		typedef vec<double, 4> vec4d;
		typedef vec<unsigned int, 4> vec4ui;


		void to_json(nlohmann::json& j, const vec4ui& p);
		void from_json(const nlohmann::json& j, vec4ui& p);

		void to_json(nlohmann::json& j, const vec4& p);
		void from_json(const nlohmann::json& j, vec4& p);

		void to_json(nlohmann::json& j, const vec4i& p);
		void from_json(const nlohmann::json& j, vec4i& p);

		void to_json(nlohmann::json& j, const vec4d& p);
		void from_json(const nlohmann::json& j, vec4d& p);
	}
}

