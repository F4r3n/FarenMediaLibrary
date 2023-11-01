#pragma once
#include "Matrix.h"
#include <nlohmann/json_fwd.hpp>


namespace fm {
	namespace math {
		typedef Matrix<float> mat;

		void to_json(nlohmann::json& j, const mat& p);
		void from_json(const nlohmann::json& j, mat& p);
	}
}
