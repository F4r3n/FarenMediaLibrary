#include "Listener.h"

using namespace fm;

Listener::Listener() {
	alListener3f(AL_POSITION, 0, 0, 1.0f);
// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
}

Listener::~Listener() {}