#include "MusicManager.h"

using namespace fm;

MusicManager MusicManager::_instance;

MusicManager::MusicManager() {}

MusicManager::~MusicManager() {}

MusicManager& MusicManager::getInstance() {
	return _instance;
}

Listener& MusicManager::getListener() {
	return listener;
}