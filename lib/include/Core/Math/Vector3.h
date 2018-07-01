#pragma once
#include <cmath>
#include <ostream>
#include "Vector.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

namespace fm {
    namespace math {
    typedef vec<float, 3> Vector3f;
    typedef vec<float, 3> vec3;

    typedef vec<int, 3> Vector3i;
    typedef vec<double, 3> Vector3d;
    typedef vec<unsigned int, 3> Vector3ui;

    template <typename T>
    void to_json(nlohmann::json& j, const vec<T,3>& p) {
            j = nlohmann::json{p.x, p.y, p.z};
        }
template <typename T>
        void from_json(const nlohmann::json& j, vec<T,3>& p) {
            p.x = j[0];
            p.y = j[1];
            p.z = j[2];
        }
    }

    }

