#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include <iostream>
Profile::Profile() {
    start = std::chrono::system_clock::now();
}

Profile::Profile(const std::string& name) {
    this->name = name;
    start = std::chrono::system_clock::now();
    
}

Profile::~Profile() {
    end = std::chrono::system_clock::now();
    Profiler::get().addProfile(this);
}