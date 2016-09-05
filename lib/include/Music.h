#pragma once 

#include "MusicManager.h"

namespace fm {
	class Music {
		static Listener& getListener() {
			return MusicManager::getInstance().getListener();
		}
		static 

	};

}