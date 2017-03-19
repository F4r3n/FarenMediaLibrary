#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include "Core/Math/Vector2.h"
namespace fm {
	class Listener {
	public:
		Listener();
		~Listener();
		void setPosition(math::Vector2f pos);
	private:
		ALfloat listenerOri[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	};

}