#pragma once
#include "Image.h"
#include "Rect.h"
#include <unordered_map>
#include <vector>
#include "Texture.h"
namespace fm
{
struct Frame
{
    float time;
    Recti rect;
    Texture texture;
};

class Animation
{
public:
    Animation(const std::string& pathSpriteSheet);
    Animation();
    ~Animation();
    void play();
    void addFrame(float time, const Recti& rect);
    const Frame& getFrame(unsigned int index) const;
    const unsigned int getSizeAnimation() const;
private:
    std::vector<Frame> frames;
    Image image;
};
}
