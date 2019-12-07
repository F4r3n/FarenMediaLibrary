#pragma once
#include <string>
#include <unordered_map>
class ScriptRegister {
public:
    static unsigned int addScript(const std::string& name) {
        if(scripts.find(name) != scripts.end())
            return scripts[name]++;
        else {
            scripts[name] = 0;
            return 0;
        }
    }

private:
    static std::unordered_map<std::string, unsigned int> scripts;
};
