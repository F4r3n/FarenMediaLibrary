#include "Music/Listener.h"

using namespace fm;

Listener::Listener() {
	alListener3f(AL_POSITION, 0, 0, 1.0f);
// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void Listener::setPosition(math::Vector2f pos) {
	alListener3f(AL_POSITION, pos.x, pos.y, 1.0f);
}

Listener::~Listener() {}