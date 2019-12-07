#pragma once
#include <unordered_map>
#include <mutex>
class Profile;

class Profiler {

public:
    Profiler();
    ~Profiler();

    static inline Profiler& get() {
        return instance;
    }

    void addProfile(Profile* profile);
    long int getTime(const std::string& name) {
        mutex.lock();
        long int t = times[name];
        mutex.unlock();
        return t;
    }

private:
    static Profiler instance;
    std::unordered_map<std::string, long int> times;
    std::mutex mutex;
};
