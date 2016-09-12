#include "Animation.h"

using namespace fm;

Animation::Animation(const std::string& pathSpriteSheet)
{
    image.loadImage(pathSpriteSheet);
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::play()
{
}

const Frame& Animation::getFrame(unsigned int index) const{
    if(index >= frames.size()) index = 0;
    return frames[index];
}

const unsigned int Animation::getSizeAnimation() const{
    return frames.size();
}

void Animation::addFrame(float time, const Recti& rect)
{
    Frame frame;
    frame.time = time;
    frame.rect = rect;

    std::vector<unsigned char> data;
    image.getPart(data, rect);
    Texture texture(data, frame.rect);

    frame.texture = texture;
    frames.push_back(frame);
}
