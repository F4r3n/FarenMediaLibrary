#include "Speaker.h"
#include <memory>
#include <iostream>
using namespace fm;
Speaker::Speaker() {
	device = alcOpenDevice(NULL);
	if (!device) {
		std::cerr << "ERROR SPEAKER : No device found" << std::endl;
	}
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		std::cerr << "Error init context" <<std::endl;
	}
	alGetError();
}

Speaker::~Speaker() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

}