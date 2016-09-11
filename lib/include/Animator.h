#pragma once

#include "Animation.h"
#include "Sprite.h"
#include <unordered_map>

namespace fm {
class Animator
{
public:
    Animator(fm::Sprite sprite);
    void process(float dt);
    void addAnimation(const std::string& name, const Animation& animation);
    ~Animator();

private:
    std::unordered_map<std::string, Animation> animations;
};
}
