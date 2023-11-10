#include "Matrix44.h"
#include "Vector4.h"
#include <nlohmann/json.hpp>


namespace fm {
	namespace math {

		template <typename T>
		void to_json(nlohmann::json& j, const Matrix<T>& p) {
			j = nlohmann::json{ p[0], p[1], p[2], p[3] };
		}
		template <typename T>
		void from_json(const nlohmann::json& j, Matrix<T>& p) {
			p[0] = j[0];
			p[1] = j[1];
			p[2] = j[2];
			p[3] = j[3];
		}

		void to_json(nlohmann::json& j, const mat& p)
		{
			to_json<float>(j, p);
		}
		void from_json(const nlohmann::json& j, mat& p)
		{
			from_json<float>(j, p);
		}
	}
}
