#include "Components/CSource.h"
//#include <sndfile.h> TODO replace by my own
#include <iostream>
using namespace fmc;
const std::string CSource::name = "Sound";

CSource::CSource() {
     alGenSources((ALuint)1, &source);
     _name = "Source";
}

CSource::CSource(const std::string& path) {
     alGenSources((ALuint)1, &source);
    loadAudio(path);
    _name = "Source";
}

CSource::~CSource() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buf);
}

void CSource::loadAudio(const std::string& path) {
    //SF_INFO info;
    //SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
    //
    //size_t read_size = 0;
    //while((read_size = sf_read_short(file, read_buf.data(), read_buf.size())) != 0) {
    //    data.insert(data.end(), read_buf.begin(), read_buf.begin() + read_size);
    //}
    //alGenBuffers(1, &buf);
    //
    //alBufferData(buf,
    //             info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
    //             &data.front(),
    //             data.size() * sizeof(uint16_t),
    //             info.samplerate);
    //
    //alSourcei(source, AL_BUFFER, buf);
    //sf_close(file);
    
}

void CSource::setLoop(bool value) {
    isLooping = value;
    alSourcei(source, AL_LOOPING, (value) ? AL_TRUE : AL_FALSE);
}

void CSource::setPitch(float pitchValue) {
    this->pitch = pitchValue;
    alSourcef(source, AL_PITCH, this->pitch);
}

void CSource::setVolume(float volume) {
    this->volume = volume;
    alSourcef(source, AL_GAIN, volume);
}

AUDIO_STATUS CSource::getStatus() {
    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    return static_cast<AUDIO_STATUS>(source_state);
}

void CSource::play() {
    alSourcePlay(source);
    ALenum error;
    if((error = alGetError()) != AL_NO_ERROR) {
        std::cerr << "Source error " << __LINE__ << " " << __FILE__ << " " << error << std::endl;
        return;
    }
    ALint source_state;

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
}