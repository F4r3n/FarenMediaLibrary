#include "Core/RandomManager.h"

RandomManager RandomManager::_instance;

RandomManager::RandomManager() {}

RandomManager::~RandomManager() {}

int RandomManager::uniform_int_distribution(int a, int b) {
    thread_local static std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(a, b);
    return uniform_dist(e1);
}

double RandomManager::uniform_real_distribution(float a, float b) {
    thread_local static std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(a, b);
    return uniform_dist(e1);
}


RandomManager& RandomManager::getInstance() {
	return _instance;
}
