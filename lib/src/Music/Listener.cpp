//#include <AL/al.h>
//#include <AL/alc.h>
#include "Music/Listener.h"

using namespace fm;

Listener::Listener() {
	//alListener3f(AL_POSITION, 0, 0, 1.0f);
	//alListener3f(AL_VELOCITY, 0, 0, 0);
	//alListenerfv(AL_ORIENTATION, listenerOri);
}

void Listener::setPosition(const math::Vector2f&) {
	//alListener3f(AL_POSITION, pos.x, pos.y, 1.0f);
}

Listener::~Listener() {}