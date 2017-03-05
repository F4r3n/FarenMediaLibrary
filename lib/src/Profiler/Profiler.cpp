#include "Profiler/Profiler.hpp"
#include "Profiler/Profile.hpp"

#include <iostream>
Profiler Profiler::instance;
Profiler::Profiler() {
}

Profiler::~Profiler() {
}

void Profiler::addProfile(Profile *profile) {
    mutex.lock();
    
    times[profile->name] = std::chrono::duration_cast<std::chrono::microseconds>(profile->end - profile->start).count();

    mutex.unlock();
}