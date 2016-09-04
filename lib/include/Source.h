#pragma once
#include <vector>
#include <map>
#include <AL/al.h>
#include <AL/alc.h>
#include "Vector2.h"

namespace fm {

	enum AUDIO_STATUS {
		INITIAL,
		PLAYING,
		PAUSED,
		STOPPED
	};
	class Source {
		public:
		Source();
		~Source();
		void loadAudio(const std::string &path);
		void play();
		void setPitch(float pitchValue);
		void setVolume(float volume);
		void setLoop(bool loop);

		void setPosition(Vector2f pos);

		AUDIO_STATUS getStatus();
	private:
		ALuint source;
		ALuint buf;
		std::vector<uint16_t> data;
		std::array<int16_t, 4096> read_buf;

		float pitch = 1.0f;
		float volume = 1.0f;
		bool isLooping = false;
	};
}