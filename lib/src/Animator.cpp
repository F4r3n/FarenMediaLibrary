#include "Animator.h"

using namespace fm;
Animator::Animator(fm::Sprite sprite) {}

void Animator::process(float dt) {}

void Animator::addAnimation(const std::string& name, const Animation& animation) {
    animations[name] = animation;
}

Animator::~Animator() {
    
}