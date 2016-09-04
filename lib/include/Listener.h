#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include "Vector2.h"
namespace fm {
	class Listener {
	public:
		Listener();
		~Listener();
		void setPosition(Vector2f pos);
	private:
		ALfloat listenerOri[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	};

}