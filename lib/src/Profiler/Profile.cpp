#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include <iostream>
Profile::Profile() {
    start = std::chrono::system_clock::now();
    running = true;
}

Profile::Profile(const std::string& name) {
    this->name = name;
    start = std::chrono::system_clock::now();
    
}

void Profile::stop() {
    end = std::chrono::system_clock::now();
    Profiler::get().addProfile(this);
    running = false;
}

Profile::~Profile() {
    if(running) {
        end = std::chrono::system_clock::now();
        Profiler::get().addProfile(this);
    }
}