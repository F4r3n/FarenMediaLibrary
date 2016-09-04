#include "Source.h"
#include <sndfile.h>
#include <iostream>
using namespace fm;
Source::Source() {
	alGenSources((ALuint)1, &source);
// check for errors

alSourcef(source, AL_PITCH, 1);
// check for errors
alSourcef(source, AL_GAIN, 1);
// check for errors
alSource3f(source, AL_POSITION, 0, 0, 0);
// check for errors
alSource3f(source, AL_VELOCITY, 0, 0, 0);
// check for errors
alSourcei(source, AL_LOOPING, AL_FALSE);
}

void Source::loadAudio(const std::string &path) {
	SF_INFO info;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
	
	size_t read_size = 0;
	while((read_size = sf_read_short(file, read_buf.data(), read_buf.size())) != 0) {
		data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
	}
	alGenBuffers(1, &buf);

	std::cout << "channel number: " << info.channels << std::endl;
	std::cout << "Sample rate: " << info.samplerate << std::endl;

	ALfloat const PITCH = 1.0f;

	alBufferData(buf, info.channels == 1? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				 &data.front(), data.size() * sizeof(uint16_t), info.samplerate);
	alSourcei(source, AL_BUFFER, buf);
	// alSourcei(src, AL_LOOPING, AL_TRUE);
	alSourcef(source, AL_PITCH, PITCH);
	
}

void Source::play() {
	alSourcePlay(source);

	ALint source_state;

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
// check for errors
	
		
        //alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        // check for errors
	
}

Source::~Source() {

}