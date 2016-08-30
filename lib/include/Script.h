#pragma once
#include <single/sol/sol.hpp>
#include "Vector2.h"
#include "InputManager.h"
namespace fm {


typedef struct Transform {
	Vector2<float> position;
	Vector2<float> scale;
	float rotation;
}Transform;

	class Script {
	public:
		Script(const std::string &script);
		Script();
		~Script();
		bool attachScript(const std::string &script);
		void update(sol::state &lua,float dt);
		void start(sol::state &lua);
		bool init(sol::state &lua);
		Transform getTransform() const;
		std::string getName() const;
		void setName(const std::string &name);

	private:
		std::string scriptName;
		std::string nameVariable;
		std::string nameFile;
		Transform transform;

		bool isDrawable = false;
	};


}