#include "Core/Math/Vector3.h"
#include <nlohmann/json.hpp>


namespace fm {
	namespace math {
	
		void to_json(nlohmann::json& j, const vec3& p) {
			j = nlohmann::json{ p.x, p.y, p.z };
		}

		void from_json(const nlohmann::json& j, vec3& p) {
			p.x = j[0];
			p.y = j[1];
			p.z = j[2];
		}
	}
}