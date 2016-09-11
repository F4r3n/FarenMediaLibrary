#include "Animation.h"

using namespace fm;

Animation::Animation(const std::string &pathSpriteSheet) {}

Animation::Animation() {}

Animation::~Animation() {}

void Animation::play() {}

void Animation::addFrame( float time, const Recti &rect) {
    Frame frame;
    frame.time = time;
    frame.rect = rect;
    frames.push_back(frame);
}
