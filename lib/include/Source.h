#pragma once


#include <vector>
#include <map>
#include <AL/al.h>
#include <AL/alc.h>
namespace fm {
	class Source {
		public:
		Source();
		~Source();
		void loadAudio(const std::string &path);
		void play();
	private:
		ALuint source;
		ALuint buf;
		std::vector<uint16_t> data;
		std::array<int16_t, 4096> read_buf;
	};
}