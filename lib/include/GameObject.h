#pragma once
#include <sol.hpp>
namespace fm {
	class GameObject {
	public:
		GameObject();
		~GameObject();
		bool attachScript(const std::string &script);
	private:
		sol::state lua;
	};


}