#ifndef __EMSCRIPTEN__
#include "Profiler/Profiler.hpp"

#endif
#include "Profiler/Profile.hpp"

Profile::Profile() {
    #ifndef __EMSCRIPTEN__
    start = std::chrono::system_clock::now();
    running = true;
    #endif
}

Profile::Profile(const std::string& name) {
    #ifndef __EMSCRIPTEN__
    this->name = name;
    start = std::chrono::system_clock::now();
    #endif
}

void Profile::stop() {
    #ifndef __EMSCRIPTEN__
    end = std::chrono::system_clock::now();
    Profiler::get().addProfile(this);
    running = false;
    #endif
}

Profile::~Profile() {
    #ifndef __EMSCRIPTEN__
    if(running) {
        end = std::chrono::system_clock::now();
        Profiler::get().addProfile(this);
    }
    #endif
}