#pragma once
#include "Vector4.h"
#include <nlohmann/json.hpp>

namespace fm {
	namespace math {

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
		
		void to_json(nlohmann::json& j, const vec4ui& p)
		{
			to_json<unsigned int>(j, p);
		}
		void from_json(const nlohmann::json& j, vec4ui& p)
		{
			from_json<unsigned int>(j, p);
		}

		void to_json(nlohmann::json& j, const vec4& p)
		{
			to_json<float>(j, p);
		}
		void from_json(const nlohmann::json& j, vec4& p)
		{
			from_json<float>(j, p);
		}

		void to_json(nlohmann::json& j, const vec4i& p)
		{
			to_json<int>(j, p);
		}
		void from_json(const nlohmann::json& j, vec4i& p)
		{
			from_json<int>(j, p);
		}

		void to_json(nlohmann::json& j, const vec4d& p)
		{
			to_json<double>(j, p);
		}
		void from_json(const nlohmann::json& j, vec4d& p)
		{
			from_json<double>(j, p);
		}

	}
}

