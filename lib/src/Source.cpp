#include "Source.h"
#include <sndfile.h>
#include <iostream>
using namespace fm;
Source::Source() {
    std::cout << "Init" << std::endl;
	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
    std::cout << "End Init" << std::endl;

}

void Source::setPosition(Vector2f pos) {
	alSource3f(source, AL_POSITION, pos.x, pos.y, 0);
}

void Source::loadAudio(const std::string &path) {
    std::cout << "LoadAudio "<< path << std::endl;
	SF_INFO info;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
	
	size_t read_size = 0;
	while((read_size = sf_read_short(file, read_buf.data(), read_buf.size())) != 0) {
		data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
	}
	alGenBuffers(1, &buf);
	alBufferData(buf, info.channels == 1? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				 &data.front(), data.size() * sizeof(uint16_t), info.samplerate);
	alSourcei(source, AL_BUFFER, buf);
	sf_close(file);
	 
}

void Source::setLoop(bool value) {
	isLooping = value;
	alSourcei(source, AL_LOOPING, (value) ? AL_TRUE : AL_FALSE);
}

void Source::setPitch(float pitchValue) {
	this->pitch = pitchValue;
	alSourcef(source, AL_PITCH, this->pitch);
}

void Source::setVolume(float volume) {
	this->volume = volume;
	alSourcef(source, AL_GAIN, volume);
}

AUDIO_STATUS Source::getStatus() {
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	return static_cast<AUDIO_STATUS>(source_state);
}

void Source::play() {
	alGetError();
	alSourcePlay(source);
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
 		std::cerr << "Source error " << __LINE__ << " " << __FILE__ << " " << error << std::endl;
 		return;
	} 
	ALint source_state;

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);	
}

Source::~Source() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buf);
}