#pragma once
#include <string>
#include <chrono>
class Profile {
    friend class Profiler;
public:
    Profile();
    Profile(const std::string& name);
    ~Profile();
    void stop();
private:
std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point end;
std::string name;
bool running = false;
};