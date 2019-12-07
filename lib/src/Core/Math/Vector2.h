#pragma once
#include <nlohmann/json_fwd.hpp>
#include "Vector.h"


namespace fm {
	namespace math {

		typedef vec<float, 2> Vector2f;
		typedef vec<int, 2> Vector2i;
		typedef vec<double, 2> Vector2d;
		typedef vec<unsigned int, 2> Vector2ui;
		typedef vec<float,2> vec2;

		void to_json(nlohmann::json& j, const vec2& p);
		void from_json(const nlohmann::json& j, vec2& p);

		//Remove use of template to accelerate compilation time

//   template <typename T>
//   void to_json(nlohmann::json& j, const vec<T,2>& p) {
//           j = nlohmann::json{p.x, p.y};
//       }
//emplate <typename T>
//       void from_json(const nlohmann::json& j, vec<T,2>& p) {
//           p.x = j[0];
//           p.y = j[1];
//       }
	}
}

