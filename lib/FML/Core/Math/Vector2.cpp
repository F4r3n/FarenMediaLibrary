#include "Core/Math/Vector2.h"
#include <nlohmann/json.hpp>


namespace fm {
	namespace math {
	
		void to_json(nlohmann::json& j, const vec2& p) {
			j = nlohmann::json{ p.x, p.y };
		}

		void from_json(const nlohmann::json& j, vec2& p) {
			p.x = j[0];
			p.y = j[1];
		}
	}
}