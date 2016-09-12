#include "Animator.h"

using namespace fm;
Animator::Animator(const fm::Sprite &sprite) : sprite(sprite) {}

void Animator::process(float dt) {
    if(hasStarted) {
        timer += dt;
        if(timer >= curentFrame.time) {
            index++;
            if(index > animations[name].getSizeAnimation()) index = 0;
            this->curentFrame = animations[name].getFrame(index);
            timer = 0;
        }
    }
}

 void Animator::play(const std::string &name) {
     this->name = name;
     this->curentFrame = animations[name].getFrame(index);
     index = 0;
     sprite.setTexture(curentFrame.texture);
     hasStarted = true;
 }

void Animator::addAnimation(const std::string& name, const Animation& animation) {
    animations[name] = animation;
}

Animator::~Animator() {
    
}