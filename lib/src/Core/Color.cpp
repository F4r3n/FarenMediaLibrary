#include "Core/Color.h"
using namespace fm;
Color::Color(float r, float g, float b)
    : r(r)
    , g(g)
    , b(b)
{
}
Color::Color(float r, float g, float b, float a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

Color::Color(int hex) {
    r = (hex >> 16) & 0xFF;
    g = (hex >> 8) & 0xFF;
    b = (hex & 0xFF);
}

float Color::mean()
{
    return (r + g + b) / 255;
}

Color::~Color()
{
}

void Color::RGB2HSV(Color &color) {
    float Rp = color.r/255;
    float Gp = color.g/255;
    float Bp = color.b/255;
    std::vector<float> elements = {Rp, Gp, Bp};
    float cmax = max(elements);
    float cmin = min(elements);
    float delta = cmax - cmin;
    color.b = cmax;
    color.g = (cmax == 0) ? 0 : delta/cmax;
    
    if(delta < 0.00001) {
        color.r = 0;
    } else if(cmax == Rp) {
        color.r = (int)(60*((Gp - Bp)/delta))%6;
    }
    else if(cmax == Gp) {
        color.r = 60*((Bp - Rp)/delta)+2;
    }
    else if(cmax == Bp) {
        color.r = 60*((Rp - Gp)/delta)+4;
    }
}

