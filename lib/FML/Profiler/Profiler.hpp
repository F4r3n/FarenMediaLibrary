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
    int64_t getTime(const std::string& name) {
        mutex.lock();
		int64_t t = times[name];
        mutex.unlock();
        return t;
    }

private:
    static Profiler instance;
    std::unordered_map<std::string, int64_t> times;
    std::mutex mutex;
};
