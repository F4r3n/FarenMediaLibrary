#pragma once
#include "Image.h"
#include "Rect.h"
#include <unordered_map>
#include <vector>
#include "Image.h"
namespace fm {
    struct Frame {
        float time;
        Recti rect;
    };
    
class Animation{
    public:
        Animation(const std::string &pathSpriteSheet);
        Animation();
        ~Animation();
        void play();
        void addFrame(float time, const Recti &rect);
    private:
        std::vector<Frame> frames;
};
}
