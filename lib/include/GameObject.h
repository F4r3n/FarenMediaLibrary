#pragma once
#include <sol.hpp>
#include "Vector2.h"
#include "InputManager.h"
namespace fm {


typedef struct Transform {
	Vector2<float> position;
	Vector2<float> scale;
	float rotation;
}Transform;

	class GameObject {
	public:
		GameObject();
		~GameObject();
		bool attachScript(const std::string &script);
		bool test();
		void update(float dt);
		Transform getTransform() const;
	private:
		sol::state lua;

		Transform transform;

		bool isDrawable = false;
	};


}