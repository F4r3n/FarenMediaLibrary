#pragma once
#include <json.hpp>
#include "Vector.h"
using nlohmann::json;

namespace fm {
	namespace math {

		typedef vec<float, 2> Vector2f;
		typedef vec<int, 2> Vector2i;
		typedef vec<double, 2> Vector2d;
		typedef vec<unsigned int, 2> Vector2ui;
		typedef vec<float,2> vec2;

    template <typename T>
    void to_json(json& j, const vec<T,2>& p) {
            j = json{{"x", p.x}, {"y", p.y}};
        }
template <typename T>
        void from_json(const json& j, vec<T,2>& p) {
            p.x = j.at("x").get<T>();
            p.y = j.at("y").get<T>();
        }
	}
}
