#pragma once

#include "Animation.h"
#include "Sprite.h"
#include <unordered_map>

namespace fm
{
class Animator
{
public:
    Animator(const fm::Sprite &sprite);
    void process(float dt);
    void addAnimation(const std::string& name, const Animation& animation);
    void play(const std::string &name);
    ~Animator();

private:
    std::unordered_map<std::string, Animation> animations;
    Sprite sprite;
    std::string name;
    
    int index = 0;
    float timer = 0.0f;
    
    Frame curentFrame;
    
    bool hasStarted = false;
};
}
