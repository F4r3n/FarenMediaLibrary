#pragma once
#include <string>
#include <chrono>
class Profile {
    #ifndef __EMSCRIPTEN__
    friend class Profiler;
    #endif
public:
    Profile();
    Profile(const std::string& name);
    ~Profile();
    void stop();
private:
#ifndef __EMSCRIPTEN__
std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point end;
std::string name;
#endif
bool running = false;
};