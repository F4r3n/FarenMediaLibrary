#pragma once
#include "InputManager.h"
namespace fm {
	class Input {
	public:
		static bool keyIsPressed(int key);
		static bool keyIsReleased(int key);
		static void getMousePosition(Vector2<double> &pos);
		static int getMouseButton(int id);
	};
}