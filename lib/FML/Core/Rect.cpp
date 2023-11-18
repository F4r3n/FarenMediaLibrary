#include "Rect.h"
#include <nlohmann/json.hpp>

namespace fm {

	template <typename T>
	void to_json(nlohmann::json& j, const Rect<T>& p) {
		j = nlohmann::json{ p.x, p.y, p.w, p.h };
	}
	template <typename T>
	void from_json(const nlohmann::json& j, Rect<T>& p) {
		p.x = j[0];
		p.y = j[1];
		p.w = j[2];
		p.h = j[3];
	}

	void to_json(nlohmann::json& j, const Rectui& p)
	{
		to_json<size_t>(j, p);
	}
	void from_json(const nlohmann::json& j, Rectui& p)
	{
		from_json<size_t>(j, p);
	}

	void to_json(nlohmann::json& j, const Rectf& p)
	{
		to_json<float>(j, p);
	}
	void from_json(const nlohmann::json& j, Rectf& p)
	{
		from_json<float>(j, p);
	}

	void to_json(nlohmann::json& j, const Recti& p)
	{
		to_json<int>(j, p);
	}
	void from_json(const nlohmann::json& j, Recti& p)
	{
		from_json<int>(j, p);
	}

	void to_json(nlohmann::json& j, const Rectd& p)
	{
		to_json<double>(j, p);
	}
	void from_json(const nlohmann::json& j, Rectd& p)
	{
		from_json<double>(j, p);
	}


}

