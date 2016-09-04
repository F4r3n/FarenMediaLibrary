#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace fm {
	class Listener {
	public:
		Listener();
		~Listener();
	private:
		ALfloat listenerOri[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	};

}